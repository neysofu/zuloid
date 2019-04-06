/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "api/jsonrpc_errors.h"
#include "cJSON/cJSON.h"
#include "chess/fen.h"
#include "chess/move.h"
#include "chess/position.h"
#include "engine.h"
#include "globals.h"
#include "time.h"
#include "utils/utils.h"

void
engine_ugei_call_stop(struct Engine *engine, cJSON *response)
{
	if (engine->mode == MODE_SEARCH) {
		cJSON_AddObjectToObject(response, "result");
	} else {
		cJSON_AddJsonRpcErrorToObject(response, JSONRPC_GENERIC_ERROR);
	}
}
