/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "engine.h"
#include "UI/cmd.h"
#include "UI/jsonrpc_errors.h"
#include "UI/methods.h"
#include "cJSON/cJSON.h"
#include "cache/cache.h"
#include "chess/position.h"
#include "core/agent.h"
#include "utils.h"
#include "xxHash/xxhash.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

char *
engine_ugei_json_call(struct Engine *engine, cJSON *request)
{
	assert(engine);
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
	} else {
		id = id ? id : cJSON_CreateNull();
		response = cJSON_CreateObject();
		cJSON_AddJsonRpcErrorToObject(
		  response, request ? JSONRPC_INVALID_REQUEST : JSONRPC_PARSE_ERROR);
	}
	cJSON_Delete(request);
	cJSON_AddItemToObject(response, "id", id);
	cJSON_AddStringToObject(response, "jsonrpc", "2.0");
	char *response_string = cJSON_PrintUnformatted(response);
	cJSON_Delete(response);
	return response_string;
}

void
engine_ugei_call(struct Engine *engine, char *string)
{
	assert(engine);
	assert(string);
	cJSON *json = cJSON_Parse(string);
	char *response = engine_ugei_json_call(engine, json);
	printf("%s\n", response);
	free(response);
}

void
engine_uci_cecp_call(struct Engine *engine, char *string)
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
		case 0xbde553e085e11527: /* "new" */
			engine->position = POSITION_DEFAULT;
			break;
		case 0x707db5f765aed6d8: /* "quit" */
			engine->mode = MODE_EXIT;
			break;
	}
}

void
engine_call(struct Engine *engine, char *string)
{
	return (*engine->caller)(engine, string);
}

void
engine_unknown_protocol_call(struct Engine *engine, char *string)
{
	assert(engine);
	cJSON *json = cJSON_Parse(string);
	if (json) {
		engine->caller = engine_ugei_call;
		engine_ugei_json_call(engine, json);
	} else {
		engine->caller = engine_uci_cecp_call;
		engine_uci_cecp_call(engine, string);
	}
}

struct Engine *
engine_new(void)
{
	struct Engine *engine = malloc(sizeof(struct Engine));
	if (engine) {
		*engine = (struct Engine){
			.position = POSITION_DEFAULT,
			.time_controls = { NULL, NULL },
			.cache = NULL,
			.agent = NULL,
			.mode = MODE_IDLE,
			.caller = engine_unknown_protocol_call,
			.port = 34290,
			.seed = 0xcfca130bUL,
			.move_selection_noise = 0.005,
			.contempt = 0.5,
			.selectivity = 0.5,
			.exit_status = EXIT_SUCCESS,
		};
	}
	return engine;
}

void
engine_delete(struct Engine *engine)
{
	if (!engine) {
		return;
	}
	time_control_delete(engine->time_controls[COLOR_WHITE]);
	time_control_delete(engine->time_controls[COLOR_BLACK]);
	cache_delete(engine->cache);
	agent_delete(engine->agent);
	free(engine);
}
