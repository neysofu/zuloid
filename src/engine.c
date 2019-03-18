/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "engine.h"
#include "cJSON/cJSON.h"
#include "cache/cache.h"
#include "chess/position.h"
#include "core/agent.h"
#include "jsonrpc_errors.h"
#include "methods.h"
#include "settings.h"
#include "utils.h"
#include "xxHash/xxhash.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

struct Engine *
engine_new(void)
{
	struct Engine *engine = malloc_or_exit(sizeof(struct Engine));
	*engine = (struct Engine){
		.position = POSITION_DEFAULT,
		.time_controls = { NULL, NULL },
		.game_clocks = { NULL, NULL },
		.settings = SETTINGS_DEFAULT,
		.cache = NULL,
		.agent = NULL,
		.notifications_stream = stdout,
		.mode = MODE_IDLE,
		.exit_status = EXIT_SUCCESS,
	};
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
	game_clock_delete(engine->game_clocks[COLOR_WHITE]);
	game_clock_delete(engine->game_clocks[COLOR_BLACK]);
	cache_delete(engine->cache);
	agent_delete(engine->agent);
	free(engine);
}

void
engine_dispatch_call(struct Engine *engine,
                     const cJSON *params,
                     cJSON *response,
                     const char *method)
{
	assert(engine);
	assert(method);
	switch (XXH64(method, strlen(method), 0)) {
		case 0x284938c798d362a8: /* "config" */
			engine_call_config(engine, params, response);
			break;
		case 0x3e6da0adb9a81aa0: /* "exit" */
			engine->mode = MODE_EXIT;
			break;
		case 0x2682377a1193c238: /* "get" */
			engine_call_get(engine, params, response);
			break;
		case 0xbbb05418c88423aa: /* "init" */
			engine_call_init(engine, params, response);
			break;
		case 0x71e6f6d1e157dbfe: /* "search" */
			engine_call_search(engine, params, response);
			break;
		case 0xde2128284d8fdb3c: /* "setup" */
			engine_call_setup(engine, params, response);
			break;
		case 0xc641b2419f8a3ce1: /* "status" */
			engine_call_status(engine, params, response);
			break;
		case 0xd8d95334d91f61fc: /* "stop" */
			engine_call_stop(engine, params, response);
			break;
		case 0xd18f9b6611eb8e16: /* "train" */
			engine_call_train(engine, params, response);
			break;
		default:
			cJSON_AddJsonRpcErrorToObject(response, JSONRPC_INVALID_METHOD);
	}
}

char *
engine_call(struct Engine *engine, const char *str)
{
	assert(engine);
	assert(str);
	cJSON *request = cJSON_Parse(str);
	cJSON *method = cJSON_GetObjectItem(request, "method");
	cJSON *params = cJSON_GetObjectItem(request, "params");
	cJSON *id = cJSON_DetachItemFromObject(request, "id");
	cJSON *response = NULL;
	if (cJSON_IsString(method)) {
		response = id ? cJSON_CreateObject() : NULL;
		engine_dispatch_call(engine, params, response, method->valuestring);
	} else if (!string_is_whitespace(str)) {
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
