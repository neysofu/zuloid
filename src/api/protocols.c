/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "api/protocols.h"
#include "api/jsonrpc_errors.h"
#include "api/methods.h"
#include "cJSON/cJSON.h"
#include "cache/cache.h"
#include "chess/position.h"
#include "core/agent.h"
#include "engine.h"
#include "globals.h"
#include "utils/dyn_str.h"
#include "utils/utils.h"
#include "xxHash/xxhash.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

/* A note on comparing strings with hashes: it makes the code more readable and
 * it's easy to add new cases. The risk of collisions is basically
 * non-existant, being it 64 bits hashes. */

void
engine_ugei(struct Engine *engine, struct DynStr *dyn_str)
{
	assert(engine);
	assert(dyn_str);
	cJSON *request = cJSON_Parse(dyn_str->buffer);
	cJSON *method = cJSON_GetObjectItem(request, "method");
	cJSON *params = cJSON_GetObjectItem(request, "params");
	cJSON *id = cJSON_DetachItemFromObject(request, "id");
	cJSON *response = NULL;
	if (cJSON_IsString(method)) {
		response = id ? cJSON_CreateObject() : NULL;
		switch (XXH64(method->valuestring, strlen(method->valuestring), 0)) {
			case 0x284938c798d362a8: /* "config" */
				engine_ugei_call_config(engine, params, response);
				break;
			case 0x3e6da0adb9a81aa0: /* "exit" */
				engine->mode = MODE_EXIT;
				break;
			case 0x2682377a1193c238: /* "get" */
				engine_ugei_call_get(engine, params, response);
				break;
			case 0xbbb05418c88423aa: /* "init" */
				engine_ugei_call_init(engine, response);
				break;
			case 0x71e6f6d1e157dbfe: /* "search" */
				engine_ugei_call_search(engine, params, response);
				break;
			case 0xde2128284d8fdb3c: /* "setup" */
				engine_ugei_call_setup(engine, params, response);
				break;
			case 0xc641b2419f8a3ce1: /* "status" */
				engine_ugei_call_status(engine, response);
				break;
			case 0xd8d95334d91f61fc: /* "stop" */
				engine_ugei_call_stop(engine, response);
				break;
			case 0xd18f9b6611eb8e16: /* "train" */
				engine_ugei_call_train(engine, params, response);
				break;
			default:
				cJSON_AddJsonRpcErrorToObject(response, JSONRPC_INVALID_METHOD);
		}
	} else if (!string_is_whitespace(dyn_str->buffer)) {
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
engine_uci(struct Engine *engine, struct DynStr *dyn_str)
{
	assert(engine);
	assert(dyn_str->buffer);
	dyn_str_tokenize(dyn_str);
	char *token = dyn_str_current_token(dyn_str);
	switch (XXH64(token, strlen(token), 0)) {
		case 0x5000d8f2907d14e4: /* "d" */
			position_print(&engine->position);
			break;
		case 0xf80028c1113b2c9c: /* "uci" */
			printf("id name Z64C\n"
			       "id author Filippo Costa\n"
			       "option name Threads type spin default 1 min 1 max 512\n"
			       "option name Clear Hash type button\n"
			       "option name Hash type spin default 8 min 0 max 65536\n"
			       "option name Ponder type check default true\n"
			       "option name Skill Level type spin default 100 min 0 max 100\n"
			       "option name Move Overhead type spin default 30 min 0 max 5000\n"
			       "option name OwnBook\n"
			       "option name UCI_Opponent\n"
			       "option name UCI_Chess960 type check default false\n"
			       "option name UCI_AnalyseMode type check default false\n"
			       "option name UCI_EngineAbout type string default %s\n"
			       "option name SyzygyPath type string default <empty>\n"
			       "option name Style type combo default normal\n"
			       "uciok\n",
			       Z64C_COPYRIGHT);
			break;
		case 0x01fd51a2a6f9cc2f: /* "debug" */
			// TODO
			break;
		case 0x33e37def10e5d195: /* "isready" */
			printf("readyok\n");
			break;
		case 0x8a7ecc4137c6f2b0: /* "position" */
			engine_uci_call_position(engine, dyn_str);
			break;
		case 0x707db5f765aed6d8: /* "quit" */
			engine->mode = MODE_EXIT;
			break;
		case 0x26cc87cdbb3247ba: /* "setoption" */
			engine_uci_call_setoption(engine, dyn_str);
			break;
		default:
			log_unknown_command(token);
	}
}

void
engine_cecp(struct Engine *engine, struct DynStr *dyn_str)
{
	assert(engine);
	assert(dyn_str->buffer);
	dyn_str_tokenize(dyn_str);
	char *token = dyn_str_current_token(dyn_str);
	switch (XXH64(token, strlen(token), 0)) {
		case 0x5000d8f2907d14e4: /* "d" */
			position_print(&engine->position);
			break;
		case 0x534feaec6d273bed: /* "ping" */
			token = dyn_str_next_token(dyn_str);
			if (token) {
				printf("ping %s\n", token);
			} else {
				printf("ping\n");
			}
			break;
		case 0x707db5f765aed6d8: /* "quit" */
			engine->mode = MODE_EXIT;
			break;
		case 0xad33154ea7fdbbc4: /* "xboard" */
			engine_cecp_call_xboard(engine, dyn_str);
			break;
	}
}

void
engine_unknown_protocol(struct Engine *engine, struct DynStr *dyn_str)
{
	assert(engine);
	assert(dyn_str->buffer);
	/* Easy and fast. We first check for "method" in JSON-RPC messages because
	 * the JSON might contain the other strings. */
	if (strstr(dyn_str->buffer, "method")) {
		engine->protocol = engine_ugei;
	} else if (strstr(dyn_str->buffer, "uci")) {
		engine->protocol = engine_uci;
	} else if (strstr(dyn_str->buffer, "xboard")) {
		engine->protocol = engine_cecp;
	} else {
		return;
	}
	engine_call(engine, dyn_str);
}
