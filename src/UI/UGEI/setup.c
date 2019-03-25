/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "cJSON/cJSON.h"
#include "chess/fen.h"
#include "chess/move.h"
#include "chess/position.h"
#include "engine.h"
#include "globals.h"
#include "UI/jsonrpc_errors.h"
#include "time.h"

void
engine_ugei_call_setup(struct Engine *engine, const cJSON *params, cJSON *response)
{
	cJSON *fen = cJSON_GetObjectItem(params, "FEN");
	cJSON *time_control_json = cJSON_GetObjectItem(params, "time_control");
	if (engine->mode != MODE_IDLE) {
		cJSON_AddJsonRpcErrorToObject(response, JSONRPC_MODE_ERROR);
		return;
	} else if (!cJSON_IsString(fen)) {
		cJSON_AddJsonRpcErrorToObject(response, JSONRPC_INVALID_PARAMS);
		return;
	}
	position_set_from_fen(&engine->position, fen->valuestring);
	engine->time_controls[COLOR_WHITE] = time_control_new_from_json(time_control_json);
	game_clock_init(&engine->game_clocks[COLOR_WHITE], engine->time_controls[COLOR_WHITE]);
	game_clock_init(&engine->game_clocks[COLOR_BLACK], engine->time_controls[COLOR_WHITE]);
}
