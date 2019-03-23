/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "protocols.h"
#include "UI/cmd.h"
#include "UI/jsonrpc_errors.h"
#include "UI/methods.h"
#include "cJSON/cJSON.h"
#include "cache/cache.h"
#include "chess/position.h"
#include "core/agent.h"
#include "engine.h"
#include "utils.h"
#include "xxHash/xxhash.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void
engine_ugei(struct Engine *engine, char *string)
{
	assert(engine);
	assert(string);
	cJSON *request = cJSON_Parse(string);
	cJSON *method = cJSON_GetObjectItem(request, "method");
	cJSON *params = cJSON_GetObjectItem(request, "params");
	cJSON *id = cJSON_DetachItemFromObject(request, "id");
	cJSON *response = NULL;
	if (cJSON_IsString(method)) {
		response = id ? cJSON_CreateObject() : NULL;
		switch (XXH64(method->valuestring, strlen(method->valuestring), 0)) {
			case 0x284938c798d362a8: /* "config" */
				engine_call_ugea_config(engine, params, response);
				break;
			case 0x3e6da0adb9a81aa0: /* "exit" */
				engine->mode = MODE_EXIT;
				break;
			case 0x2682377a1193c238: /* "get" */
				engine_call_ugea_get(engine, params, response);
				break;
			case 0xbbb05418c88423aa: /* "init" */
				engine_call_ugea_init(engine, params, response);
				break;
			case 0x71e6f6d1e157dbfe: /* "search" */
				engine_call_ugea_search(engine, params, response);
				break;
			case 0xde2128284d8fdb3c: /* "setup" */
				engine_call_ugea_setup(engine, params, response);
				break;
			case 0xc641b2419f8a3ce1: /* "status" */
				engine_call_ugea_status(engine, params, response);
				break;
			case 0xd8d95334d91f61fc: /* "stop" */
				engine_call_ugea_stop(engine, params, response);
				break;
			case 0xd18f9b6611eb8e16: /* "train" */
				engine_call_ugea_train(engine, params, response);
				break;
			default:
				cJSON_AddJsonRpcErrorToObject(response, JSONRPC_INVALID_METHOD);
		}
	} else if (!string_is_whitespace(string)) {
		id = id ? id : cJSON_CreateNull();
		response = cJSON_CreateObject();
		cJSON_AddJsonRpcErrorToObject(
		  response, request ? JSONRPC_INVALID_REQUEST : JSONRPC_PARSE_ERROR);
	}
	cJSON_Delete(request);
	cJSON_AddItemToObject(response, "id", id);
	cJSON_AddStringToObject(response, "jsonrpc", "2.0");
	char *response_string = cJSON_PrintUnformatted(response);
	if (response_string) {
		printf("%s\n", response_string);
	}
	free(response_string);
	cJSON_Delete(response);
}

void
engine_uci_or_cecp(struct Engine *engine, char *string)
{
	assert(engine);
	assert(string);
	struct Cmd cmd;
	cmd_init(&cmd, string);
	char *token = cmd_at(&cmd, 0);
	switch (XXH64(token, strlen(token), 0)) {
		case 0xf80028c1113b2c9c: /* "uci" */
			printf("id name Z64C\n"
			       "id author Filippo Costa\n"
			       "uciok\n");
			break;
		case 0x33e37def10e5d195: /* "isready" */
			printf("readyok\n");
			break;
		case 0x707db5f765aed6d8: /* "quit" */
			engine->mode = MODE_EXIT;
			break;
		case 0xad33154ea7fdbbc4: /* "xboard" */
			printf("feature myname=\"Z64C\"\n"
			       "feature ping=1\n");
			break;
		case 0x534feaec6d273bed: /* "ping" */
			printf("ping %s\n", cmd_next(&cmd));
			break;
	}
}

void
engine_unknown_protocol(struct Engine *engine, char *string)
{
	assert(engine);
	assert(string);
	size_t i = strspn(string, WHITESPACE_CHARS);
	size_t j = i + strcspn(string + i, WHITESPACE_CHARS);
	char terminating_character = string[j];
	string[j] = '\0';
	if (strcmp(string + i, "uci") == 0 || strcmp(string + i, "xboard") == 0) {
		engine->protocol = engine_uci_or_cecp;
		engine_call(engine, string);
	} else {
		string[j] = terminating_character;
		cJSON *json = cJSON_Parse(string);
		if (json) {
			engine->protocol = engine_ugei;
			engine_call(engine, string);
		}
		cJSON_Delete(json);
	}
}
