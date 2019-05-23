/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "cJSON/cJSON.h"
#include "cache/cache.h"
#include "chess/position.h"
#include "core/agent.h"
#include "engine.h"
#include "globals.h"
#include "utils.h"
#include "xxHash/xxhash.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

/* === UTILS === */

enum JsonRpcError
{
	/* Predefined. */
	JSONRPC_PARSE_ERROR = -32700,
	JSONRPC_INVALID_PARAMS = -32602,
	JSONRPC_INVALID_METHOD = -32601,
	JSONRPC_INVALID_REQUEST = -32600,
	/* UGEI-specific. */
	JSONRPC_GENERIC_ERROR = 0,
	JSONRPC_OOM = 21,
	JSONRPC_UNSUPPORTED_RULESET = 900,
	JSONRPC_UNDEFINED_KEY = 902,
	JSONRPC_MODE_ERROR = 1400,
};

const char *
jsonrpc_error_default_message(enum JsonRpcError error)
{
	switch (error) {
		case JSONRPC_PARSE_ERROR:
			return "Parse error";
		case JSONRPC_INVALID_PARAMS:
			return "Invalid params";
		case JSONRPC_INVALID_METHOD:
			return "Invalid method";
		case JSONRPC_INVALID_REQUEST:
			return "Invalid Request";
		case JSONRPC_GENERIC_ERROR:
			return "Generic error";
		case JSONRPC_OOM:
			return "Out-of-memory condition";
		case JSONRPC_UNSUPPORTED_RULESET:
			return "Unsupported ruleset";
		case JSONRPC_UNDEFINED_KEY:
			return "Undefined key";
		case JSONRPC_MODE_ERROR:
			return "Mode error";
		default:
			return "Generic error";
	}
}

cJSON *
cJSON_AddJsonRpcErrorToObject(cJSON *object, enum JsonRpcError error)
{
	if (!object) {
		return NULL;
	}
	const char *message = jsonrpc_error_default_message(error);
	cJSON *message_item = cJSON_CreateStringReference(message);
	cJSON *error_item = cJSON_AddObjectToObject(object, "error");
	cJSON_AddNumberToObject(error_item, "code", error);
	cJSON_AddItemToObject(error_item, "message", message_item);
	return error_item;
}

/* === END UTILS === */

void
engine_ugei_call_config(struct Engine *engine, const cJSON *params, cJSON *response)
{
	cJSON *key = cJSON_GetObjectItem(params, "key");
	cJSON *value = cJSON_GetObjectItem(params, "value");
	if (engine->mode != MODE_IDLE) {
		cJSON_AddJsonRpcErrorToObject(response, JSONRPC_MODE_ERROR);
		return;
	} else if (!cJSON_IsString(key)) {
		cJSON_AddJsonRpcErrorToObject(response, JSONRPC_INVALID_PARAMS);
		return;
	}
	switch (XXH64(key->valuestring, strlen(key->valuestring), 0)) {
		case 0x6e87002190572330: /* "seed" */
			engine->seed = rand();
			break;
		case 0x46f9bde5468e310a: /* "move_selection_noise" */
			engine->move_selection_noise = value->valuedouble;
			break;
		case 0x597a99f05012dd35: /* "contempt" */
			engine->contempt = value->valuedouble;
			break;
		case 0x92cfb5ce943dee0d: /* "selectivity" */
			engine->selectivity = value->valuedouble;
			break;
		default:
			cJSON_AddJsonRpcErrorToObject(response, JSONRPC_UNDEFINED_KEY);
	}
}

void
engine_ugei_call_get(struct Engine *engine, const cJSON *params, cJSON *response)
{
	cJSON *value;
	cJSON *key = cJSON_GetObjectItem(params, "key");
	if (!cJSON_IsObject(params) || !cJSON_IsString(key)) {
		cJSON_AddJsonRpcErrorToObject(response, JSONRPC_INVALID_PARAMS);
		return;
	}
	switch (XXH64(key->valuestring, strlen(key->valuestring), 0)) {
		case 0x63f596aaf1c3bed7: /* FEN */
			value = cJSON_CreateString(fen_new_from_position(&engine->position));
			break;
		case 0x46f9bde5468e310a: /* "move_selection_noise" */
			value = cJSON_CreateNumber(engine->move_selection_noise);
			break;
		case 0x597a99f05012dd35: /* "contempt" */
			value = cJSON_CreateNumber(engine->contempt);
			break;
		case 0x92cfb5ce943dee0d: /* "selectivity" */
			value = cJSON_CreateNumber(engine->selectivity);
			break;
		default:
			cJSON_AddJsonRpcErrorToObject(response, JSONRPC_UNDEFINED_KEY);
			return;
	}
	cJSON *result = cJSON_AddObjectToObject(response, "result");
	cJSON_AddItemToObject(result, "value", value);
	return;
}

void
engine_ugei_call_init(struct Engine *engine, cJSON *response)
{
	engine->agent = agent_new();
	engine->time_controls[COLOR_WHITE] = time_control_new_bullet();
	game_clock_init(&engine->game_clocks[COLOR_WHITE], engine->time_controls[COLOR_WHITE]);
	game_clock_init(&engine->game_clocks[COLOR_BLACK], engine->time_controls[COLOR_WHITE]);
	if (!engine->agent) {
		cJSON_AddJsonRpcErrorToObject(response, JSONRPC_GENERIC_ERROR);
		return;
	}
	cJSON *result = cJSON_AddObjectToObject(response, "result");
	cJSON *meta = cJSON_AddObjectToObject(result, "meta");
	cJSON_AddStringToObject(meta, "copyright", Z64C_COPYRIGHT);
	cJSON_AddStringToObject(meta, "license", Z64C_LICENSE);
	cJSON_AddStringToObject(meta, "name", "Z64C");
	cJSON_AddStringToObject(meta, "release_date", Z64C_BUILD_DATE);
	cJSON_AddStringToObject(meta, "url", Z64C_URL);
	cJSON_AddStringToObject(meta, "version", Z64C_VERSION);
	cJSON *rulesets = cJSON_AddArrayToObject(result, "rulesets");
	cJSON_AddItemToArray(rulesets, cJSON_CreateString("chess"));
}

void
engine_ugei_call_search(struct Engine *engine, const cJSON *params, cJSON *response)
{
	if (engine->mode != MODE_IDLE) {
		cJSON_AddJsonRpcErrorToObject(response, JSONRPC_MODE_ERROR);
		return;
	}
}

void
engine_ugei_call_train(struct Engine *engine, const cJSON *params, cJSON *response)
{
	return;
}

void
engine_ugei_call_stop(struct Engine *engine, cJSON *response)
{
	if (engine->mode == MODE_SEARCH) {
		cJSON_AddObjectToObject(response, "result");
	} else {
		cJSON_AddJsonRpcErrorToObject(response, JSONRPC_GENERIC_ERROR);
	}
}

void
engine_ugei_call_status(struct Engine *engine, cJSON *response)
{
	char *mode;
	switch (engine->mode) {
		case MODE_IDLE:
			mode = "idle";
		case MODE_SEARCH:
			mode = "searching";
		case MODE_EXIT:
			mode = "exiting";
		default:
			mode = "?";
	}
	cJSON *result = cJSON_AddObjectToObject(response, "result");
	cJSON_AddStringToObject(result, "mode", mode);
}

void
engine_ugei(struct Engine *engine, char *cmd)
{
	assert(engine);
	assert(cmd);
	cJSON *request = cJSON_Parse(cmd);
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
		/* TODO: Do not print any errors if the command is empty. */
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
	if (response_string) {
		printf("%s\n", response_string);
	}
	free(response_string);
	cJSON_Delete(response);
}
