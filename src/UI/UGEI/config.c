/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * This file implements the external APIs to the engine over stdin. */

#include "cJSON/cJSON.h"
#include "chess/fen.h"
#include "chess/move.h"
#include "chess/position.h"
#include "engine.h"
#include "globals.h"
#include "UI/jsonrpc_errors.h"
#include "time.h"
#include "utils.h"
#include "xxHash/xxhash.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

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
