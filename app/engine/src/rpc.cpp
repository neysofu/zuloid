/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <iostream>
#include <json/single_include/nlohmann/json.hpp>

extern "C" {

#include "chess/board.h"
#include "chess/fen.h"
#include "chess/legality.h"
#include "chess/move.h"
#include "clock.h"
#include "driver.h"
#include "game.h"
#include "log.h"
#include "mt19937-64/mt64.h"
#include "search/scontroller.h"
#include "settings.h"
#include "utils.h"
#include "xxHash.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sysexits.h>
}

using namespace nlohmann;

#define MAX_PERFT_DEPTH 16

const std::string uci_jsonrpc_version = "2.0";
const std::string field_code = "code";
const std::string field_error = "error";
const std::string field_id = "id";
const std::string field_jsonrpc = "jsonrpc";
const std::string field_message = "message";
const std::string field_method = "method";
const std::string field_params = "params";
const std::string field_result = "result";

/** 'uci' method. */
const std::string field_meta = "meta";
const std::string field_name = "name";
const std::string field_authors = "author(s)";
const std::string field_release_date = "release_date";
const std::string field_license = "license";
const std::string field_url = "url";
const std::string field_version = "version";

/** 'get' method. */
const std::string field_key = "key";
const std::string field_value = "value";

/** 'load' method. */
const std::string field_path = "path";

/** 'setup' method. */
const std::string field_game = "game";
const std::string field_notation = "notation";
const std::string field_data = "data";
const std::string field_tree = "tree";

/** Error codes and messages. */
const int32_t uci_parse_error_code = -32700;
const std::string uci_parse_error_message = "Parse error.";
const int32_t uci_invalid_request_code = -32600;
const std::string uci_invalid_request_message = "Invalid request.";
const int32_t uci_method_not_found_code = -32601;
const std::string uci_method_not_found_message = "Method not found.";
const int32_t uci_invalid_params_code = -32602;
const std::string uci_invalid_params_message = "Invalid parameters.";
const int32_t uci_operational_mode_error_code = 100;
const int32_t uci_unsupported_error = 101;
const std::string uci_operational_mode_error_message =
  "Unavailable method during search time.";
const int32_t uci_invalid_chess_code = 150;
const std::string uci_invalid_chess_message = "Invalid notation.";

json
driver_rpc_load(struct Driver *driver, json request, bool must_respond)
{
	assert(driver);
	json response;
	if (driver->mode != MODE_IDLE) {
		response[field_error][field_code] = uci_operational_mode_error_code;
		response[field_error][field_message] = uci_operational_mode_error_message;
		return response;
	}
	if (request.count(field_params) == 0 ||
	    request[field_params].count(field_path) == 0) {
		response[field_error][field_code] = uci_invalid_params_code;
		response[field_error][field_message] = uci_invalid_params_message;
		return response;
	}
	char *path = strdup(request[field_params][field_path].get<std::string>().c_str());
	/** TODO: ttable_load(driver->ttable, path); */
	free(path);
	return response;
}

json
driver_rpc_perft(struct Driver *driver, json request, bool must_respond)
{
	assert(driver);
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
	// board_legal_moves(&driver->board, &legal_moves);
	// for (counter = 0; counter < depth; counter++) {
	//	ttable_insert(ttables, board_buffer);
	//}
	// free(ttables);
	return json::object();
}

json
driver_rpc_get(struct Driver *driver, json request, bool must_respond)
{
	assert(driver);
	json response;
	if (request.count(field_params) == 0 ||
	    request[field_params].count(field_key) == 0) {
		json response;
		response[field_error][field_code] = uci_invalid_params_code;
		response[field_error][field_message] = uci_invalid_params_message;
		return response;
	}
	std::string key = request[field_params][field_key];
	json value;
	char *buffer;
	switch (XXH64(key.c_str(), key.size(), 0)) {
		case 0x99bfb67800f8ab48: /** "meta:name" */
			value = engine_name;
			break;
		case 0x0eedec02bc43b1c6: /** "meta:version" */
			value = engine_version;
			break;
		case 0xd3ed40bbb9d9813d: /** "meta:release-date" */
			value = engine_release_date;
			break;
		case 0x8d15e178965e9e80: /** "meta:author(s)" */
			value = engine_author;
			break;
		case 0xbe96813f393d45c8: /** "meta:license" */
			value = engine_license;
			break;
		case 0x9f39919c36cf7b1d: /** "meta:url" */
			value = engine_url;
			break;
		case 0x3dde2c8e2f1ae60c: /** "position:fen" */
			buffer = (char *)xmalloc(FEN_SIZE);
			board_to_fen(&driver->board, buffer);
			value = buffer;
			free(buffer);
			break;
		case 0xe0c8b500ae055972: /** "position:result" */
			if (driver->result.termination == TERMINATION_NONE) {
				value = nullptr;
			} else {
				switch (driver->result.winner) {
					case COLOR_WHITE:
						value = 2;
						break;
					case COLOR_BLACK:
						value = 0;
						break;
					default:
						value = 1;
						break;
				}
			}
			break;
		case 0x6ef4a701e3e2a582: /** "position:castling" */
			value["WK"] =
			  bool(driver->board.game_state & GAME_STATE_CASTLING_RIGHT_WK);
			value["BK"] =
			  bool(driver->board.game_state & GAME_STATE_CASTLING_RIGHT_WK);
			value["WQ"] =
			  bool(driver->board.game_state & GAME_STATE_CASTLING_RIGHT_WK);
			value["BQ"] =
			  bool(driver->board.game_state & GAME_STATE_CASTLING_RIGHT_WK);
			break;
		case 0x29e651ba535a6cff: /** "position:turn" */
			if (board_active_color(&driver->board) == COLOR_WHITE) {
				value = 0;
			} else {
				value = 1;
			}
			break;
		case 0xe255add3561ac54e: /** "position:en-passant" */
			if (!board_en_passant_is_available(&driver->board)) {
				value = nullptr;
			} else {
				value = "e4";
			}
			break;
		case 0x1edf70458ca59bce: /** "settings:port" */
			value = driver->settings.port;
			break;
		case 0x2162fe7692a5051f: /** "settings:ponder" */
			value = driver->settings.ponder;
			break;
		case 0x76718e771ec28df5: /** "settings:train" */
			value = driver->settings.train;
			break;
		case 0x292c66878d4413f6: /** "settings:debug" */
			value = driver->settings.debug;
			break;
		case 0x451f838c8458ea73: /** "settings:max-cache-size" */
			value = driver->settings.max_cache_size;
			break;
		case 0x066a44fc3b92d2ed: /** "settings:move-selection-noise" */
			value = driver->settings.move_selection_noise;
			break;
		case 0x5f81485bdd5a5258: /** "settings:resign-rate" */
			value = driver->settings.resign_rate;
			break;
		case 0x1c454dcd57990902: /** "settings:resoluteness" */
			value = driver->settings.resoluteness;
			break;
		case 0x614e9ea3558ed27c: /** "settings:selectivity" */
			value = driver->settings.selectivity;
			break;
		default:
			response[field_error][field_code] = uci_invalid_params_code;
			response[field_error][field_message] = uci_invalid_params_message;
			return response;
	}
	response[field_result][field_key] = request[field_params][field_key];
	response[field_result][field_value] = value;
	return response;
}

json
driver_rpc_exit(struct Driver *driver, json request, bool must_respond)
{
	assert(driver);
	driver->exit_status = EX_OK;
	driver->mode = MODE_EXIT;
	return json::object();
}

json
driver_rpc_save(struct Driver *driver, json request, bool must_respond)
{
	assert(driver);
	json response;
	if (driver->mode != MODE_IDLE) {
		response[field_error][field_code] = uci_operational_mode_error_code;
		response[field_error][field_message] = uci_operational_mode_error_message;
		return response;
	}
	if (request.count(field_params) == 0 ||
	    request[field_params].count(field_path) == 0) {
		response[field_error][field_code] = uci_invalid_params_code;
		response[field_error][field_message] = uci_invalid_params_message;
		return response;
	}
	char *path = strdup(request[field_params][field_path].get<std::string>().c_str());
	/** TODO: ttable_save(driver->ttable, path); */
	free(path);
	return response;
}

json
driver_rpc_search(struct Driver *driver, json request, bool must_respond)
{
	assert(driver);
	// char *arg_1 = cmd_arg_ith(cmd, 1);
	// if (strcmp(arg_1, "start") == 0) {
	//	scontroller_start(driver->scontroller, &driver->board);
	//	// Move best_move = ttable_best_move(driver->ttable);
	//} else if (strcmp(arg_1, "stop") == 0) {
	//	if (driver->mode != MODE_BUSY) {
	//		response["error"]["code"] = UCI_ERROR_CODE_MODE;
	//		response["error"]["message"] = "Z64C can't stop the search.";
	//	}
	//	driver->mode = MODE_IDLE;
	//}
	return json::object();
}

json
driver_rpc_set(struct Driver *driver, json request, bool must_respond)
{
	assert(driver);
	// if (driver->mode != MODE_IDLE) {
	//	response["errors"]["code"] = UCI_ERROR_CODE_MODE;
	//	response["errors"]["message"] = "Blach.";
	//	return response;
	//}
	// char *cmd_arg_2 = cmd_arg_ith(cmd, 2);
	// switch (XXH64(cmd_arg_1, strlen(cmd_arg_1), 0)) {
	//	case 0x6e87002190572330: /** "seed" */
	//		driver->settings.seed = genrand64_int64() >> 32;
	//		return 0;
	//}
	// if (!(cmd_arg_1 && cmd_arg_2)) {
	//	fprintf(stderr, "error \"Invalid syntax. Usage: 'set [name]
	//[value]'.\"\n"); 	return EX_USAGE;
	//}
	// if (settings_set_value(&driver->settings, cmd_arg_1, cmd_arg_2) == -1) {
	//	response["error"]["code"] = -32000;
	//	response["error"]["message"] = "";
	//} else {
	//	response["result"] = {};
	//}
	return json::object();
}

json
driver_rpc_setup(struct Driver *driver, json request, bool must_respond)
{
	assert(driver);
	json response;
	if (driver->mode != MODE_IDLE) {
		response[field_error][field_code] = uci_operational_mode_error_code;
		response[field_error][field_message] = uci_operational_mode_error_message;
		return response;
	}
	if (response.count(field_params) != 1 ||
	    !response[field_params][field_game].is_string()) {
		response[field_error][field_code] = uci_invalid_params_code;
		response[field_error][field_message] = uci_invalid_params_message;
		return response;
	}
	std::string game_name = request[field_params].value(field_game, "chess");
	auto data = request[field_params][field_data];
	switch (XXH64(game_name.c_str(), game_name.size(), 0)) {
		// case 0xab434cbcb7b79499: /** "960" */
		//	board_setup_960(&driver->board, 960);
		// case 0x63f596aaf1c3bed7: /** "FEN" */
		//	fen_to_board(data.c_str(), &driver->board);
		//	break;
		case 0x24a20c7fb35220c8: /** "initial" */
			driver->board = BOARD_STARTPOS;
			break;
	}
	// size_t i = 3;
	// while ((move_as_str = cmd_arg_ith(cmd, i))) {
	//	Move move = str_to_move(move_as_str);
	//	enum Piece piece = board_piece_at(&driver->board, move_source(move));
	//	if (!board_approves_legal_move(&driver->board,
	//								   move,
	//								   piece)) {
	//		fprintf(stdout, "error \"One or more of the given moves are "
	//			   "illegal (%s).\"\n",
	//			   move_as_str);
	//		return EX_DATAERR;
	//	}
	//	driver->result = board_push_legal_move(&driver->board, move, piece);
	//}
	return response;
}

json
driver_rpc_uci(struct Driver *driver, json request, bool must_respond)
{
	assert(driver);
	LOG_DEBUG("UCI session initialization via 'uci' command.\n");
	json response;
	if (must_respond) {
		response[field_result][field_meta][field_license] = engine_license;
		response[field_result][field_meta][field_name] = engine_name;
		response[field_result][field_meta][field_authors] = engine_author;
		response[field_result][field_meta][field_version] = engine_version;
		response[field_result][field_meta][field_release_date] =
		  engine_release_date;
		response[field_result][field_meta][field_url] = engine_url;
	}
	return response;
}

extern "C" {

const char *
driver_rpc(struct Driver *driver, const char *cmd)
{
	LOG_DEBUG("New JSON RPC request.\n");
	json request;
	json response;
	try {
		request = json::parse(cmd);
	} catch (json::parse_error &e) {
		LOG_WARN("The JSON RPC request is not parsable.\n");
		response = {
			{ field_jsonrpc, uci_jsonrpc_version },
			{ field_error,
			  {
			    { field_code, uci_parse_error_code },
			    { field_message, uci_parse_error_message },
			  } },
			{ field_id, nullptr },
		};
		return strdup(response.dump().c_str());
	}
	LOG_DEBUG("Running sanity checks on the JSON RPC request.\n");
	if (request.count(field_method) == 0 || !request[field_method].is_string() ||
	    request.count(field_jsonrpc) == 0 ||
	    !request[field_jsonrpc].is_string() ||
	    request[field_jsonrpc].get<std::string>() != uci_jsonrpc_version) {
		LOG_WARN("Sanity checks failed.\n");
		response = {
			{ field_jsonrpc, uci_jsonrpc_version },
			{ field_error,
			  {
			    { field_code, uci_invalid_request_code },
			    { field_message, uci_invalid_request_message },
			  } },
			{ field_id, nullptr },
		};
		return strdup(response.dump().c_str());
	}
	bool has_id = request.count("id") == 1;
	LOG_DEBUG(
	  "The JSON RPC request is a notification. No response will be sent.\n");
	std::string method_name = request["method"];
	LOG_DEBUG("Doing method dispatching for the JSON RPC request.\n");
	switch (XXH64(method_name.c_str(), method_name.size(), 0)) {
		case 0xa2aa05ed9085aaf9: /** "foobar" */
			/** Just give back an empty result. */
			break;
		case 0x2682377a1193c238: /** "get" */
			response = driver_rpc_get(driver, request, has_id);
			break;
		case 0x757dfc0052b6ac66: /** "load" */
			response = driver_rpc_load(driver, request, has_id);
			break;
		case 0xec22b9453ad51a5a: /** "move" */
			// response = driver_rpc_move(driver, request, must_respond);
			break;
		case 0x3e6da0adb9a81aa0: /** "exit" */
			response = driver_rpc_exit(driver, request, has_id);
			break;
#if Z64C_SWITCH_TRAIN
		case 0xd18f9b6611eb8e16: /** "train" */
			response = driver_rpc_train(driver, request, has_id);
			break;
#endif
		case 0xd795d67f2e9e6996: /** "save" */
			response = driver_rpc_save(driver, request, has_id);
			break;
		case 0x71e6f6d1e157dbfe: /** "search" */
			response = driver_rpc_search(driver, request, has_id);
			break;
		case 0x24115d14507a61eb: /** "set" */
			response = driver_rpc_set(driver, request, has_id);
			break;
		case 0xde2128284d8fdb3c: /** "setup" */
			response = driver_rpc_setup(driver, request, has_id);
			break;
		case 0xf80028c1113b2c9c: /** "uci" */
			response = driver_rpc_uci(driver, request, has_id);
			break;
		default:
			LOG_WARN("The JSON RPC method was not found.\n");
			response = { { field_error,
				             { { field_code, uci_method_not_found_code },
				               { field_message, uci_method_not_found_message } } } };
			break;
	}
	if (has_id) {
		response[field_jsonrpc] = uci_jsonrpc_version;
		/** Add a dummy result if the computation has returns no value. */
		if (response.count(field_result) == 0 && response.count(field_error) == 0) {
			response[field_result] = json::object();
		}
		response[field_id] = request[field_id];
		return strdup(response.dump().c_str());
	} else {
		LOG_DEBUG("No response given.\n");
		return NULL;
	}
}
}
