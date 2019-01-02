/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * This file implements the external APIs to the engine over stdin. */

#include "UGEI/errors.h"
#include "UGEI/property_names.h"
#include "cJSON/cJSON.h"
#include "chess/fen.h"
#include "chess/move.h"
#include "chess/position.h"
#include "engine.h"
#include "globals.h"
#include "settings.h"
#include "time.h"
#include "utils.h"
#include "xxHash/xxhash.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void
engine_call_config(struct Engine *engine,
                   const struct cJSON *params,
                   struct cJSON *response)
{
	struct cJSON *key = cJSON_GetObjectItem(params, PROPERTY_NAME_KEY);
	struct cJSON *value = cJSON_GetObjectItem(params, PROPERTY_NAME_VALUE);
	struct cJSON *error = NULL;
	if (engine->mode != MODE_IDLE || !(key && value)) {
		cJSON_AddItemToObject(
		  response, PROPERTY_NAME_ERROR, cJSON_CreateJsonRpcError(JSONRPC_INVALID_PARAMS));
		return;
	}
	switch (XXH64(key->valuestring, strlen(key->valuestring), 0)) {
		case 0x6e87002190572330: /* "seed" */
			engine->settings.seed = rand();
			break;
		case 0x46f9bde5468e310a: /* "move_selection_noise" */
			engine->settings.move_selection_noise = value->valuedouble;
			break;
		case 0x597a99f05012dd35: /* "contempt" */
			engine->settings.contempt = value->valuedouble;
			break;
		case 0x92cfb5ce943dee0d: /* "selectivity" */
			engine->settings.selectivity = value->valuedouble;
			break;
		default:
			cJSON_AddItemToObject(response,
			                      PROPERTY_NAME_ERROR,
			                      cJSON_CreateJsonRpcError(JSONRPC_UNDEFINED_KEY));
	}
}
