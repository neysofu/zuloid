/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "cJSON/cJSON.h"
#include "engine.h"
#include "UI/jsonrpc_errors.h"
#include "utils.h"

void
engine_ugei_call_search(struct Engine *engine, const cJSON *params, cJSON *response)
{
	if (engine->mode != MODE_IDLE) {
		cJSON_AddJsonRpcErrorToObject(response, JSONRPC_MODE_ERROR);
		return;
	}
	struct GameClock *gc = &engine->game_clocks[engine->position.side_to_move];
	float available_time_in_seconds = game_clock_start(gc);
	cJSON *result = cJSON_AddObjectToObject(response, "result");
	cJSON *evaluation = cJSON_AddNumberToObject(result, "cp", 0);
}
