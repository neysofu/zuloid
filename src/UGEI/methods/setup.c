/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "UGEI/errors.h"
#include "UGEI/property_names.h"
#include "cJSON/cJSON.h"
#include "chess/fen.h"
#include "chess/move.h"
#include "chess/position.h"
#include "engine.h"
#include "globals.h"
#include "time.h"

void
engine_call_setup(struct Engine *engine, const struct cJSON *params, struct cJSON *response)
{
	struct cJSON *fen = cJSON_GetObjectItem(params, PROPERTY_NAME_FEN);
	struct cJSON *time_control = cJSON_GetObjectItem(params, PROPERTY_NAME_TIME_CONTROL);
	if (engine->mode != MODE_IDLE) {
		cJSON_AddJsonRpcErrorToObject(response, JSONRPC_GENERIC_ERROR);
	} else if (!cJSON_IsString(fen)) {
		cJSON_AddJsonRpcErrorToObject(response, JSONRPC_INVALID_PARAMS);
	} else {
		position_set_from_fen(&engine->position, fen->valuestring);
		cJSON_AddObjectToObject(response, PROPERTY_NAME_RESULT);
	}
}
