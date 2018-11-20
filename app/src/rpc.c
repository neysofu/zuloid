/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "cJSON/cJSON.h"
#include "chess/board.h"
#include "chess/fen.h"
#include "chess/legality.h"
#include "chess/move.h"
#include "clock.h"
#include "engine.h"
#include "game.h"
#include "globals.h"
#include "mt19937-64/mt64.h"
#include "settings.h"
#include "trace.h"
#include "utils.h"
#include "xxHash.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sysexits.h>

#define MAX_PERFT_DEPTH 16

static const char *jsonrpc_version = "2.0";
static const char *field_code = "code";
static const char *field_error = "error";
static const char *field_id = "id";
static const char *field_jsonrpc = "jsonrpc";
static const char *field_message = "message";
static const char *field_method = "method";
static const char *field_params = "params";
static const char *field_result = "result";

/** 'uci' method. */
static const char *field_meta = "meta";
static const char *field_name = "name";
static const char *field_authors = "author(s)";
static const char *field_release_date = "release-date";
static const char *field_license = "license";
static const char *field_url = "url";
static const char *field_version = "version";

/** 'get' method. */
static const char *field_key = "key";
static const char *field_value = "value";

/** 'load' method. */
static const char *field_path = "path";

/** 'setup' method. */
static const char *field_game = "game";
static const char *field_notation = "notation";
static const char *field_data = "data";
static const char *field_tree = "tree";

/** Error codes and messages. */
static const int32_t uci_parse_error_code = -32700;
static const char *uci_parse_error_message = "Parse error.";
static const int32_t uci_invalid_request_code = -32600;
static const char *uci_invalid_request_message = "Invalid request.";
static const int32_t uci_method_not_found_code = -32601;
static const char *uci_method_not_found_message = "Method not found.";
static const int32_t uci_invalid_params_code = -32602;
static const char *uci_invalid_params_message = "Invalid parameters.";
static const int32_t uci_operational_mode_error_code = 100;
static const int32_t uci_unsupported_error = 101;
static const char *uci_operational_mode_error_message =
  "Unavailable method during search time.";
static const int32_t uci_invalid_chess_code = 150;
static const char *uci_invalid_chess_message = "Invalid notation.";

struct cJSON *
engine_rpc_load(struct Engine *engine, struct cJSON *params)
{
	assert(engine);
	cJSON *response = cJSON_CreateObject();
	cJSON *body = cJSON_CreateObject();
	//if (engine->mode != MODE_IDLE) {
	//	return util_jsonrpc_error(uci_operational_mode_error_code,
	//	                          uci_operational_mode_error_message);
	//}
	//if (!cJSON_HasObjectItem(params, field_path)) {
	//	return util_jsonrpc_error(uci_invalid_params_code, uci_invalid_params_message);
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
	return cJSON_AddObjectToObject(cJSON_CreateObject(), field_result);
}

struct cJSON *
engine_rpc_get(struct Engine *engine, const struct cJSON *params)
{
	assert(engine);
	struct cJSON *response = cJSON_CreateObject();
	struct cJSON *content = cJSON_CreateObject();
	if (!params || !cJSON_IsObject(params) ||
	    !cJSON_IsString(cJSON_GetObjectItem(params, field_key))) {
		cJSON_AddNumberToObject(content, field_code, uci_invalid_params_code);
		cJSON_AddStringToObject(content, field_message, uci_invalid_params_message);
		cJSON_AddItemToObject(response, field_error, content);
		return response;
	}
	const char *key = cJSON_GetObjectItem(params, field_key)->valuestring;
	/**
	 * Note that value only allocates memory if a match is found. No cleanup on
	 * errors is needed.
	 */
	struct cJSON *value;
	char *buffer;
	switch (XXH64(key, strlen(key), 0)) {
		case 0x99bfb67800f8ab48: /** "meta:name" */
			value = cJSON_CreateString(engine_name);
			break;
		case 0x0eedec02bc43b1c6: /** "meta:version" */
			value = cJSON_CreateString(engine_version);
			break;
		case 0xd3ed40bbb9d9813d: /** "meta:release-date" */
			value = cJSON_CreateString(engine_release_date);
			break;
		case 0x8d15e178965e9e80: /** "meta:author(s)" */
			value = cJSON_CreateString(engine_author);
			break;
		case 0xbe96813f393d45c8: /** "meta:license" */
			value = cJSON_CreateString(engine_license);
			break;
			break;
		case 0x9f39919c36cf7b1d: /** "meta:url" */
			value = cJSON_CreateString(engine_url);
			break;
		case 0x3dde2c8e2f1ae60c: /** "position:fen" */
			buffer = xmalloc(FEN_SIZE);
			board_to_fen(&engine->board, buffer);
			value = cJSON_CreateString(buffer);
			free(buffer);
			break;
		case 0xe0c8b500ae055972: /** "position:result" */
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
		case 0x6ef4a701e3e2a582: /** "position:castling" */
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
		case 0x29e651ba535a6cff: /** "position:turn" */
			if (board_active_color(&engine->board) == COLOR_WHITE) {
				value = cJSON_CreateNumber(0);
			} else {
				value = cJSON_CreateNumber(1);
			}
			break;
		case 0xe255add3561ac54e: /** "position:en-passant" */
			if (!board_en_passant_is_available(&engine->board)) {
				value = cJSON_CreateNull();
			} else {
				value = cJSON_CreateString("e4");
			}
			break;
		case 0x1edf70458ca59bce: /** "settings:port" */
			value = cJSON_CreateNumber(engine->settings.port);
			break;
		case 0x2162fe7692a5051f: /** "settings:ponder" */
			value = cJSON_CreateBool(engine->settings.ponder);
			break;
		case 0x76718e771ec28df5: /** "settings:train" */
			value = cJSON_CreateBool(engine->settings.train);
			break;
		case 0x292c66878d4413f6: /** "settings:debug" */
			value = cJSON_CreateBool(engine->settings.debug);
			break;
		case 0x451f838c8458ea73: /** "settings:max-cache-size" */
			value = cJSON_CreateNumber(engine->settings.max_cache_size);
			break;
		case 0x066a44fc3b92d2ed: /** "settings:move-selection-noise" */
			value = cJSON_CreateNumber(engine->settings.move_selection_noise);
			break;
		case 0x5f81485bdd5a5258: /** "settings:resign-rate" */
			value = cJSON_CreateNumber(engine->settings.resign_rate);
			break;
		case 0x1c454dcd57990902: /** "settings:resoluteness" */
			value = cJSON_CreateNumber(engine->settings.resoluteness);
			break;
		case 0x614e9ea3558ed27c: /** "settings:selectivity" */
			value = cJSON_CreateNumber(engine->settings.selectivity);
			break;
		default:
			cJSON_AddNumberToObject(content, field_code, uci_invalid_params_code);
			cJSON_AddStringToObject(content, field_message, uci_invalid_params_message);
			cJSON_AddItemToObject(response, field_error, content);
			return response;
	}
	/** Let's finally assembly the response object. */
	cJSON_AddItemToObject(content, field_key, cJSON_GetObjectItem(params, field_key));
	cJSON_AddItemToObject(content, field_value, value);
	cJSON_AddItemToObject(response, field_result, content);
	return response;
}

struct cJSON *
engine_rpc_exit(struct Engine *engine, struct cJSON *params)
{
	assert(engine);
	engine->exit_status = EX_OK;
	engine->mode = MODE_EXIT;
	struct cJSON *response = cJSON_CreateObject();
	cJSON_AddObjectToObject(response, field_result);
	return response;
}

struct cJSON *
engine_rpc_status(struct Engine *engine, struct cJSON *params)
{
	assert(engine);
	struct cJSON *response = cJSON_CreateObject();
	cJSON_AddObjectToObject(response, field_result);
	return response;
}

struct cJSON *
engine_rpc_save(struct Engine *engine, struct cJSON *params)
{
	struct cJSON *response = cJSON_CreateObject();
	cJSON_AddObjectToObject(response, field_result);
	return response;
	// if (engine->mode != MODE_IDLE) {
	//	cJSON *error = cJSON_CreateObject();
	//	cJSON_AddItemToObject(
	//	  error, field_code, cJSON_CreateNumber(uci_operational_mode_error_code));
	//	cJSON_AddItemToObject(
	//	  error,
	//	  field_message,
	//	  cJSON_CreateString(uci_operational_mode_error_message));
	//	cJSON_AddItemToObject(response, field_error, error);
	//	return response;
	//}
	// if (!cJSON_HasObjectItem(request, field_params) ||
	//    request[field_params].count(field_path) == 0) {
	//	cJSON *error = cJSON_CreateObject();
	//	cJSON_AddItemToObject(
	//	  error, field_code, cJSON_CreateNumber(uci_invalid_params_code));
	//	cJSON_AddItemToObject(
	//	  error, field_message, cJSON_CreateString(uci_invalid_params_message));
	//	cJSON_AddItemToObject(response, field_error, error);
	//	return response;
	//}
	return response;
}

struct cJSON *
engine_rpc_search(struct Engine *engine, struct cJSON *params)
{
	struct cJSON *response = cJSON_CreateObject();
	cJSON_AddObjectToObject(response, field_result);
	return response;
}

struct cJSON *
engine_rpc_set(struct Engine *engine, struct cJSON *params)
{
	struct cJSON *response = cJSON_CreateObject();
	struct cJSON *content = cJSON_CreateObject();
	if (engine->mode != MODE_IDLE) {
		cJSON_AddNumberToObject(content, field_code, uci_invalid_params_code);
		cJSON_AddStringToObject(content, field_message, uci_invalid_params_message);
		cJSON_AddItemToObject(response, field_error, content);
		return response;
	} else if (!cJSON_IsString(cJSON_GetObjectItem(content, field_key))) {
	}
	const char *key = cJSON_GetObjectItem(content, field_key)->valuestring;
	struct cJSON *value = cJSON_GetObjectItem(content, field_value);
	switch (XXH64(key, strlen(key), 0)) {
		case 0x6e87002190572330: /** "seed" */
			engine->settings.seed = genrand64_int64() >> 32;
			return 0;
	}
	if (settings_set_value(&engine->settings, key, value) == -1) {
		cJSON_AddNumberToObject(content, field_code, uci_operational_mode_error_code);
		cJSON_AddStringToObject(content, field_message, uci_operational_mode_error_message);
		cJSON_AddItemToObject(response, field_error, content);
		return response;
	} else {
		cJSON_AddItemToObject(response, field_result, content);
	}
	cJSON_AddItemToObject(response, field_result, content);
	return response;
}

struct cJSON *
engine_rpc_setup(struct Engine *engine, struct cJSON *params)
{
	struct cJSON *response = cJSON_CreateObject();
	struct cJSON *content = cJSON_CreateObject();
	if (engine->mode != MODE_IDLE) {
		cJSON_AddNumberToObject(content, field_code, uci_operational_mode_error_code);
		cJSON_AddStringToObject(content, field_message, uci_operational_mode_error_message);
		cJSON_AddItemToObject(response, field_error, content);
		return response;
	}
	if (!cJSON_IsObject(cJSON_GetObjectItem(response, field_params))) {
		cJSON_AddNumberToObject(content, field_code, uci_operational_mode_error_code);
		cJSON_AddStringToObject(content, field_message, uci_operational_mode_error_message);
		cJSON_AddItemToObject(response, field_error, content);
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
engine_rpc_uci(struct Engine *engine, struct cJSON *params)
{
	TRACE("UCI session initialization via 'uci' command.\n");
	struct cJSON *response = cJSON_CreateObject();
	struct cJSON *result = cJSON_AddObjectToObject(response, field_result);
	struct cJSON *meta = cJSON_AddObjectToObject(result, field_meta);
	cJSON_AddStringToObject(meta, field_license, engine_license);
	cJSON_AddStringToObject(meta, field_name, engine_name);
	cJSON_AddStringToObject(meta, field_authors, engine_author);
	cJSON_AddStringToObject(meta, field_version, engine_version);
	cJSON_AddStringToObject(meta, field_release_date, engine_release_date);
	cJSON_AddStringToObject(meta, field_url, engine_url);
	return response;
}

const char *
engine_rpc(struct Engine *engine, const char *cmd)
{
	TRACE("Parsing the JSON RPC request.\n");
	const struct cJSON *request = cJSON_Parse(cmd);
	struct cJSON *response = cJSON_CreateObject();
	struct cJSON *error;
	struct cJSON *id = cJSON_CreateNull();
	if (!request || cJSON_GetErrorPtr()) {
		TRACE("The JSON RPC request is not parsable.\n");
		error = cJSON_AddObjectToObject(response, field_error);
		cJSON_AddNumberToObject(error, field_code, uci_parse_error_code);
		cJSON_AddStringToObject(error, field_message, uci_parse_error_message);
		goto respond;
	}
	if (!cJSON_IsString(cJSON_GetObjectItem(request, field_method))) {
		TRACE("The JSON RPC request is parsable but semantically incorrect.\n");
		error = cJSON_AddObjectToObject(response, field_error);
		cJSON_AddNumberToObject(error, field_code, uci_invalid_request_code);
		cJSON_AddStringToObject(error, field_message, uci_invalid_request_message);
		goto respond;
	}
	TRACE("The JSON RPC request is valid. Now proceeding to method dispatching.\n");
	id = cJSON_GetObjectItem(request, field_id);
	const struct cJSON *params = cJSON_GetObjectItem(request, field_params);
	const char *method = cJSON_GetObjectItem(request, field_method)->valuestring;
	switch (XXH64(method, strlen(method), 0)) {
		case 0xc641b2419f8a3ce1: /** "status" */
			response = engine_rpc_status(engine, params);
			break;
		case 0x2682377a1193c238: /** "get" */
			response = engine_rpc_get(engine, params);
			break;
		case 0x757dfc0052b6ac66: /** "load" */
			response = engine_rpc_load(engine, params);
			break;
		case 0xec22b9453ad51a5a: /** "move" */
			// response = engine_rpc_move(engine, params, must_respond);
			break;
		case 0x3e6da0adb9a81aa0: /** "exit" */
			response = engine_rpc_exit(engine, params);
			break;
#if Z64C_SWITCH_TRAIN
		case 0xd18f9b6611eb8e16: /** "train" */
			response = engine_rpc_train(engine, params);
			break;
#endif
		case 0xd795d67f2e9e6996: /** "save" */
			response = engine_rpc_save(engine, params);
			break;
		case 0x71e6f6d1e157dbfe: /** "search" */
			response = engine_rpc_search(engine, params);
			break;
		case 0x24115d14507a61eb: /** "set" */
			response = engine_rpc_set(engine, params);
			break;
		case 0xde2128284d8fdb3c: /** "setup" */
			response = engine_rpc_setup(engine, params);
			break;
		case 0xf80028c1113b2c9c: /** "uci" */
			response = engine_rpc_uci(engine, params);
			break;
		default:
			TRACE("Method dispatch failed.\n");
			error = cJSON_AddObjectToObject(response, field_error);
			cJSON_AddNumberToObject(error, field_code, uci_method_not_found_code);
			cJSON_AddStringToObject(error, field_message, uci_method_not_found_message);
			goto respond;
	}
	TRACE("Method dispatching was successful and the response object is ready.\n");
	if (id) {
respond:
		cJSON_AddStringToObject(response, field_jsonrpc, jsonrpc_version);
		cJSON_AddItemToObject(response, field_id, id);
		const char *response_str = cJSON_PrintUnformatted(response);
		cJSON_Delete(response);
		return response_str;
	} else {
		return NULL;
	}
}
