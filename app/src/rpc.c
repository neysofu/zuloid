/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This file implements the external APIs to the engine over stdin. */

#include "rpc.h"
#include "cJSON/cJSON.h"
#include "chess/board.h"
#include "chess/fen.h"
#include "chess/legality.h"
#include "chess/move.h"
#include "clock.h"
#include "debug.h"
#include "engine.h"
#include "game.h"
#include "globals.h"
#include "mt19937-64/mt64.h"
#include "settings.h"
#include "utils.h"
#include "xxHash.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sysexits.h>

struct cJSON *
engine_rpc_load(struct Engine *engine, const struct cJSON *params)
{
	assert(engine);
	struct cJSON *response = cJSON_CreateObject();
	struct cJSON *body = cJSON_CreateObject();
	// if (engine->mode != MODE_IDLE) {
	//	return util_jsonrpc_error(JSONRPC_MODE_ERROR_CODE,
	//	                          JSONRPC_MODE_ERROR_MSG);
	//}
	// if (!cJSON_HasObjectItem(params, field_path)) {
	//	return util_jsonrpc_error(JSONRPC_INVALID_PARAMS_CODE, JSONRPC_INVALID_PARAMS_MSG);
	//}
	return response;
}

struct cJSON *
engine_rpc_perft(struct Engine *engine, const struct cJSON *params)
{
	assert(engine);
	// size_t depth = atoi(cmd_arg_ith(cmd, 1));
	//// Error detection.
	// if (depth == 0 && strcmp(cmd_arg_ith(cmd, 1), "0") != 0) {
	//	/** {"errors" : [{"code" : 101}]} */
	//	return UCI_ERROR_INVALID_PARAMS;
	//}
	// if (depth > MAX_PERFT_DEPTH) {
	//	return "";
	//}
	// struct TTable *ttables = xmalloc(sizeof(struct TTable *) * depth);
	// struct Board *board_buffer = xmalloc(sizeof(struct Board));
	//*board_buffer = BOARD_STARTPOS;
	// Move legal_moves[BOARD_MAX_NUM_LEGAL_MOVES];
	// size_t counter;
	// board_legal_moves(&engine->board, &legal_moves);
	// for (counter = 0; counter < depth; counter++) {
	//	ttable_insert(ttables, board_buffer);
	//}
	// free(ttables);
	return cJSON_AddObjectToObject(cJSON_CreateObject(), LITERAL_RESULT);
}

struct cJSON *
engine_rpc_get(struct Engine *engine, const struct cJSON *params)
{
	assert(engine);
	struct cJSON *response = cJSON_CreateObject();
	struct cJSON *content = cJSON_CreateObject();
	if (!params || !cJSON_IsObject(params) ||
	    !cJSON_IsString(cJSON_GetObjectItem(params, LITERAL_KEY))) {
		cJSON_AddNumberToObject(content, LITERAL_CODE, JSONRPC_INVALID_PARAMS_CODE);
		cJSON_AddStringToObject(content, LITERAL_MESSAGE, JSONRPC_INVALID_PARAMS_MSG);
		cJSON_AddItemToObject(response, LITERAL_ERROR, content);
		return response;
	}
	const char *key = cJSON_GetObjectItem(params, LITERAL_KEY)->valuestring;
	/* Note that value only allocates memory if a match is found. No cleanup on
	 * errors is needed. */
	struct cJSON *value;
	char *buffer;
	switch (XXH64(key, strlen(key), 0)) {
		case 0x99bfb67800f8ab48: /* "meta:name" */
			value = cJSON_CreateString(SOFTWARE_NAME);
			break;
		case 0x0eedec02bc43b1c6: /* "meta:version" */
			value = cJSON_CreateString(SOFTWARE_VERSION);
			break;
		case 0xd3ed40bbb9d9813d: /* "meta:release-date" */
			value = cJSON_CreateString(SOFTWARE_RELEASE_DATE);
			break;
		case 0x8d15e178965e9e80: /* "meta:author(s)" */
			value = cJSON_CreateString(SOFTWARE_AUTHOR);
			break;
		case 0xbe96813f393d45c8: /* "meta:license" */
			value = cJSON_CreateString(SOFTWARE_LICENSE);
			break;
			break;
		case 0x9f39919c36cf7b1d: /* "meta:url" */
			value = cJSON_CreateString(SOFTWARE_URL);
			break;
		case 0x3dde2c8e2f1ae60c: /* "position:fen" */
			buffer = xmalloc(FEN_SIZE);
			board_to_fen(&engine->board, buffer);
			value = cJSON_CreateString(buffer);
			free(buffer);
			break;
		case 0xe0c8b500ae055972: /* "position:result" */
			if (engine->result.termination == TERMINATION_NONE) {
				value = cJSON_CreateNull();
			} else {
				switch (engine->result.winner) {
					case COLOR_WHITE:
						value = cJSON_CreateNumber(2);
						break;
					case COLOR_BLACK:
						value = cJSON_CreateNumber(0);
						break;
					default:
						value = cJSON_CreateNumber(1);
						break;
				}
			}
			break;
		case 0x6ef4a701e3e2a582: /* "position:castling" */
			value = cJSON_CreateObject();
			cJSON_AddBoolToObject(
			  value, "WK", engine->board.game_state & GAME_STATE_CASTLING_RIGHT_WK);
			cJSON_AddBoolToObject(
			  value, "BK", engine->board.game_state & GAME_STATE_CASTLING_RIGHT_WK);
			cJSON_AddBoolToObject(
			  value, "WQ", engine->board.game_state & GAME_STATE_CASTLING_RIGHT_WK);
			cJSON_AddBoolToObject(
			  value, "BQ", engine->board.game_state & GAME_STATE_CASTLING_RIGHT_WK);
			break;
		case 0x29e651ba535a6cff: /* "position:turn" */
			if (board_active_color(&engine->board) == COLOR_WHITE) {
				value = cJSON_CreateNumber(0);
			} else {
				value = cJSON_CreateNumber(1);
			}
			break;
		case 0xe255add3561ac54e: /* "position:en-passant" */
			if (!board_en_passant_is_available(&engine->board)) {
				value = cJSON_CreateNull();
			} else {
				value = cJSON_CreateString("e4");
			}
			break;
		case 0x1edf70458ca59bce: /* "settings:port" */
			value = cJSON_CreateNumber(engine->settings.port);
			break;
		case 0x451f838c8458ea73: /* "settings:max-cache-size" */
			value = cJSON_CreateNumber(engine->settings.max_cache_size);
			break;
		case 0x066a44fc3b92d2ed: /* "settings:move-selection-noise" */
			value = cJSON_CreateNumber(engine->settings.move_selection_noise);
			break;
		case 0x5f81485bdd5a5258: /* "settings:resign-rate" */
			value = cJSON_CreateNumber(engine->settings.resign_rate);
			break;
		case 0x1c454dcd57990902: /* "settings:resoluteness" */
			value = cJSON_CreateNumber(engine->settings.resoluteness);
			break;
		case 0x614e9ea3558ed27c: /* "settings:selectivity" */
			value = cJSON_CreateNumber(engine->settings.selectivity);
			break;
		default:
			cJSON_AddNumberToObject(content, LITERAL_CODE, JSONRPC_INVALID_PARAMS_CODE);
			cJSON_AddStringToObject(content, LITERAL_MESSAGE, JSONRPC_INVALID_PARAMS_MSG);
			cJSON_AddItemToObject(response, LITERAL_ERROR, content);
			return response;
	}
	/* Let's finally assembly the response object. */
	cJSON_AddItemToObject(content, LITERAL_KEY, cJSON_GetObjectItem(params, LITERAL_KEY));
	cJSON_AddItemToObject(content, LITERAL_VALUE, value);
	cJSON_AddItemToObject(response, LITERAL_RESULT, content);
	return response;
}

struct cJSON *
engine_rpc_exit(struct Engine *engine, const struct cJSON *params)
{
	assert(engine);
	engine->exit_status = EX_OK;
	engine->mode = MODE_EXIT;
	struct cJSON *response = cJSON_CreateObject();
	cJSON_AddObjectToObject(response, LITERAL_RESULT);
	return response;
}

struct cJSON *
engine_rpc_status(struct Engine *engine, const struct cJSON *params)
{
	assert(engine);
	struct cJSON *response = cJSON_CreateObject();
	cJSON_AddObjectToObject(response, LITERAL_RESULT);
	return response;
}

struct cJSON *
engine_rpc_save(struct Engine *engine, const struct cJSON *params)
{
	struct cJSON *response = cJSON_CreateObject();
	cJSON_AddObjectToObject(response, LITERAL_RESULT);
	return response;
	// if (engine->mode != MODE_IDLE) {
	//	cJSON *error = cJSON_CreateObject();
	//	cJSON_AddItemToObject(
	//	  error, LITERAL_CODE, cJSON_CreateNumber(JSONRPC_MODE_ERROR_CODE));
	//	cJSON_AddItemToObject(
	//	  error,
	//	  LITERAL_MESSAGE,
	//	  cJSON_CreateString(JSONRPC_MODE_ERROR_MSG));
	//	cJSON_AddItemToObject(response, LITERAL_ERROR, error);
	//	return response;
	//}
	// if (!cJSON_HasObjectItem(request, LITERAL_PARAMS) ||
	//    request[LITERAL_PARAMS].count(field_path) == 0) {
	//	cJSON *error = cJSON_CreateObject();
	//	cJSON_AddItemToObject(
	//	  error, LITERAL_CODE, cJSON_CreateNumber(JSONRPC_INVALID_PARAMS_CODE));
	//	cJSON_AddItemToObject(
	//	  error, LITERAL_MESSAGE, cJSON_CreateString(JSONRPC_INVALID_PARAMS_MSG));
	//	cJSON_AddItemToObject(response, LITERAL_ERROR, error);
	//	return response;
	//}
	return response;
}

struct cJSON *
engine_rpc_search(struct Engine *engine, const struct cJSON *params)
{
	struct cJSON *response = cJSON_CreateObject();
	cJSON_AddObjectToObject(response, LITERAL_RESULT);
	return response;
}

struct cJSON *
engine_rpc_set(struct Engine *engine, const struct cJSON *params)
{
	struct cJSON *response = cJSON_CreateObject();
	struct cJSON *content = cJSON_CreateObject();
	if (engine->mode != MODE_IDLE) {
		cJSON_AddNumberToObject(content, LITERAL_CODE, JSONRPC_INVALID_PARAMS_CODE);
		cJSON_AddStringToObject(content, LITERAL_MESSAGE, JSONRPC_INVALID_PARAMS_MSG);
		cJSON_AddItemToObject(response, LITERAL_ERROR, content);
		return response;
	} else if (!cJSON_IsString(cJSON_GetObjectItem(content, LITERAL_KEY))) {
	}
	const char *key = cJSON_GetObjectItem(content, LITERAL_KEY)->valuestring;
	struct cJSON *value = cJSON_GetObjectItem(content, LITERAL_VALUE);
	switch (XXH64(key, strlen(key), 0)) {
		case 0x6e87002190572330: /** "seed" */
			engine->settings.seed = genrand64_int64() >> 32;
			return 0;
	}
	if (settings_set_value(&engine->settings, key, value) == -1) {
		cJSON_AddNumberToObject(content, LITERAL_CODE, JSONRPC_MODE_ERROR_CODE);
		cJSON_AddStringToObject(content, LITERAL_MESSAGE, JSONRPC_MODE_ERROR_MSG);
		cJSON_AddItemToObject(response, LITERAL_ERROR, content);
		return response;
	} else {
		cJSON_AddItemToObject(response, LITERAL_RESULT, content);
	}
	cJSON_AddItemToObject(response, LITERAL_RESULT, content);
	return response;
}

struct cJSON *
engine_rpc_setup(struct Engine *engine, const struct cJSON *params)
{
	struct cJSON *response = cJSON_CreateObject();
	struct cJSON *content = cJSON_CreateObject();
	if (engine->mode != MODE_IDLE) {
		cJSON_AddNumberToObject(content, LITERAL_CODE, JSONRPC_MODE_ERROR_CODE);
		cJSON_AddStringToObject(content, LITERAL_MESSAGE, JSONRPC_MODE_ERROR_MSG);
		cJSON_AddItemToObject(response, LITERAL_ERROR, content);
		return response;
	}
	if (!cJSON_IsObject(cJSON_GetObjectItem(response, LITERAL_PARAMS))) {
		cJSON_AddNumberToObject(content, LITERAL_CODE, JSONRPC_MODE_ERROR_CODE);
		cJSON_AddStringToObject(content, LITERAL_MESSAGE, JSONRPC_MODE_ERROR_MSG);
		cJSON_AddItemToObject(response, LITERAL_ERROR, content);
		return response;
	}
	char *game_name = response->valuestring; // FIXME
	switch (XXH64(game_name, strlen(game_name), 0)) {
		// case 0xab434cbcb7b79499: /** "960" */
		//	board_setup_960(&engine->board, 960);
		// case 0x63f596aaf1c3bed7: /** "FEN" */
		//	fen_to_board(data.c_str(), &engine->board);
		//	break;
		case 0x24a20c7fb35220c8: /** "initial" */
			engine->board = BOARD_STARTPOS;
			break;
	}
	return response;
}

struct cJSON *
engine_rpc_train(struct Engine *engine, const struct cJSON *params)
{
	assert(engine);
	struct cJSON *response = cJSON_CreateObject();
	cJSON_AddObjectToObject(response, LITERAL_RESULT);
	return response;
}

struct cJSON *
engine_rpc_uci(struct Engine *engine, const struct cJSON *params)
{
	DEBUG("UCI session initialization via 'uci' command.");
	struct cJSON *response = cJSON_CreateObject();
	struct cJSON *result = cJSON_AddObjectToObject(response, LITERAL_RESULT);
	struct cJSON *meta = cJSON_AddObjectToObject(result, LITERAL_META);
	cJSON_AddStringToObject(meta, LITERAL_LICENSE, SOFTWARE_LICENSE);
	cJSON_AddStringToObject(meta, LITERAL_NAME, SOFTWARE_NAME);
	cJSON_AddStringToObject(meta, LITERAL_AUTHORS, SOFTWARE_AUTHOR);
	cJSON_AddStringToObject(meta, LITERAL_VERSION, SOFTWARE_VERSION);
	cJSON_AddStringToObject(meta, LITERAL_RELEASE_DATE, SOFTWARE_RELEASE_DATE);
	cJSON_AddStringToObject(meta, LITERAL_URL, SOFTWARE_URL);
	return response;
}

const char *
engine_rpc(struct Engine *engine, const char *cmd)
{
	DEBUG("Parsing the JSON RPC request.\n");
	const struct cJSON *request = cJSON_Parse(cmd);
	struct cJSON *response = cJSON_CreateObject();
	struct cJSON *error;
	struct cJSON *id = cJSON_CreateNull();
	/* Be aware of threading issues with cJSON_GetErrorPtr. No problem here
	 * because it is single-threaded. */
	if (!request || cJSON_GetErrorPtr()) {
		DEBUG("The JSON RPC request is not parsable.");
		error = cJSON_AddObjectToObject(response, LITERAL_ERROR);
		cJSON_AddNumberToObject(error, LITERAL_CODE, JSONRPC_PARSE_ERROR_CODE);
		cJSON_AddStringToObject(error, LITERAL_MESSAGE, JSONRPC_PARSE_ERROR_MSG);
		goto respond;
	}
	if (!cJSON_IsString(cJSON_GetObjectItem(request, LITERAL_METHOD))) {
		DEBUG("The JSON RPC request is parsable but semantically incorrect.");
		error = cJSON_AddObjectToObject(response, LITERAL_ERROR);
		cJSON_AddNumberToObject(error, LITERAL_CODE, JSONRPC_INVALID_REQUEST_CODE);
		cJSON_AddStringToObject(error, LITERAL_MESSAGE, JSONRPC_INVALID_REQUEST_MSG);
		goto respond;
	}
	DEBUG("The JSON RPC request is valid. Now proceeding to method dispatching.");
	id = cJSON_GetObjectItem(request, LITERAL_ID);
	const struct cJSON *params = cJSON_GetObjectItem(request, LITERAL_PARAMS);
	const char *method = cJSON_GetObjectItem(request, LITERAL_METHOD)->valuestring;
	switch (XXH64(method, strlen(method), 0)) {
		case 0xc641b2419f8a3ce1: /* "status" */
			response = engine_rpc_status(engine, params);
			break;
		case 0x2682377a1193c238: /* "get" */
			response = engine_rpc_get(engine, params);
			break;
		case 0x757dfc0052b6ac66: /* "load" */
			response = engine_rpc_load(engine, params);
			break;
		case 0x3e6da0adb9a81aa0: /* "exit" */
			response = engine_rpc_exit(engine, params);
			break;
		case 0xd18f9b6611eb8e16: /* "train" */
			response = engine_rpc_train(engine, params);
			break;
		case 0xd795d67f2e9e6996: /* "save" */
			response = engine_rpc_save(engine, params);
			break;
		case 0x71e6f6d1e157dbfe: /* "search" */
			response = engine_rpc_search(engine, params);
			break;
		case 0x24115d14507a61eb: /* "set" */
			response = engine_rpc_set(engine, params);
			break;
		case 0xde2128284d8fdb3c: /* "setup" */
			response = engine_rpc_setup(engine, params);
			break;
		case 0xf80028c1113b2c9c: /* "uci" */
			response = engine_rpc_uci(engine, params);
			break;
		default:
			DEBUG("Method dispatch failed.\n");
			error = cJSON_AddObjectToObject(response, LITERAL_ERROR);
			cJSON_AddNumberToObject(error, LITERAL_CODE, JSONRPC_INVALID_METHOD_CODE);
			cJSON_AddStringToObject(error, LITERAL_MESSAGE, JSONRPC_INVALID_PARAMS_MSG);
			goto respond;
	}
	DEBUG("Method dispatching was successful and the response object is ready.");
	if (id) {
	respond:
		cJSON_AddStringToObject(response, LITERAL_JSONRPC, LITERAL_TWO_POINT_OH);
		cJSON_AddItemToObject(response, LITERAL_ID, id);
		const char *response_str = cJSON_PrintUnformatted(response);
		cJSON_Delete(response);
		return response_str;
	} else {
		return NULL;
	}
}
