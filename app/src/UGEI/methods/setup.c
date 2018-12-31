/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "UGEI/errors.h"
#include "UGEI/property_names.h"
#include "cJSON/cJSON.h"
#include "chess/position.h"
#include "chess/fen.h"
#include "chess/move.h"
#include "engine.h"
#include "globals.h"
#include "time.h"

struct cJSON *
engine_call_setup(struct Engine *engine, const struct cJSON *params)
{
	struct cJSON *response = cJSON_CreateObject();
	if (engine->mode != MODE_IDLE) {
		cJSON_AddItemToObject(response,
		                      PROPERTY_NAME_ERROR,
		                      cJSON_CreateJsonRpcError(JSONRPC_GENERIC_ERROR));
		return response;
	}
	if (!cJSON_IsObject(params)) {
		cJSON_AddItemToObject(response,
		                      PROPERTY_NAME_ERROR,
		                      cJSON_CreateJsonRpcError(JSONRPC_INVALID_PARAMS));
		return response;
	}
	const struct cJSON *fen = cJSON_GetObjectItem(params, PROPERTY_NAME_FEN);
	const struct cJSON *time_control = cJSON_GetObjectItem(params, PROPERTY_NAME_TIME_CONTROL);
	if (fen) {
		position_set_from_fen(&engine->position, fen->valuestring);
	}
	return response;
}
