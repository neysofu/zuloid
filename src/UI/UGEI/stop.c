/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "UI/jsonrpc_errors.h"
#include "cJSON/cJSON.h"
#include "chess/fen.h"
#include "chess/move.h"
#include "chess/position.h"
#include "engine.h"
#include "globals.h"
#include "time.h"
#include "utils.h"

void
engine_call_ugea_stop(struct Engine *engine, const cJSON *params, cJSON *response)
{
	UNUSED(params);
	if (engine->mode == MODE_SEARCH) {
		cJSON_AddObjectToObject(response, "result");
	} else {
		cJSON_AddJsonRpcErrorToObject(response, JSONRPC_GENERIC_ERROR);
	}
}
