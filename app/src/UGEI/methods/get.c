/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "UGEI/errors.h"
#include "UGEI/property_names.h"
#include "cJSON/cJSON.h"
#include "chess/fen.h"
#include "chess/position.h"
#include "engine.h"
#include "globals.h"
#include "settings.h"
#include "utils.h"
#include "xxHash/xxhash.h"
#include <string.h>

struct cJSON *
engine_call_get(struct Engine *engine, const struct cJSON *params)
{
	struct cJSON *response = cJSON_CreateObject();
	struct cJSON *error;
	struct cJSON *result;
	struct cJSON *value;
	const struct cJSON *key = cJSON_GetObjectItem(params, PROPERTY_NAME_KEY);
	if (!cJSON_IsObject(params) || !cJSON_IsString(key)) {
		error = cJSON_AddObjectToObject(response, PROPERTY_NAME_RESULT);
		// cJSON_AddNumberToObject(error, PROPERTY_NAME_CODE,
		// PROPERTY_NAME_INVALID_PARAMS_CODE); cJSON_AddStringToObject(error,
		// PROPERTY_NAME_MESSAGE, PROPERTY_NAME_INVALID_PARAMS_MESSAGE);
		return response;
	}
	char *buffer;
	switch (XXH64(key->valuestring, strlen(key->valuestring), 0)) {
		case 0x63f596aaf1c3bed7: /* FEN */
			buffer = fen_new_from_position(&engine->position);
			value = cJSON_CreateString(buffer);
			break;
		case 0x919aee5c3985d5bc: /* "result" */
			if (engine->termination == TERMINATION_NONE) {
				value = cJSON_CreateNull();
			} else {
				switch (engine->winner) {
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
		case 0xadd81cf390e81050: /* "cache_block_size" */
			value = cJSON_CreateNumber(engine->settings.cache_block_size);
			break;
		case 0xdb8e268abf5db364: /* "max_cache_blocks_count" */
			value = cJSON_CreateNumber(engine->settings.max_cache_blocks_count);
			break;
		case 0x46f9bde5468e310a: /* "move_selection_noise" */
			value = cJSON_CreateNumber(engine->settings.move_selection_noise);
			break;
		case 0x597a99f05012dd35: /* "contempt" */
			value = cJSON_CreateNumber(engine->settings.contempt);
			break;
		case 0x92cfb5ce943dee0d: /* "selectivity" */
			value = cJSON_CreateNumber(engine->settings.selectivity);
			break;
		default:
			cJSON_AddItemToObject(response,
			                      PROPERTY_NAME_ERROR,
			                      cJSON_CreateJsonRpcError(JSONRPC_UNDEFINED_KEY));
			// cJSON_AddNumberToObject(error, PROPERTY_NAME_CODE,
			// PROPERTY_NAME_UNDEFINED_CODE);  cJSON_AddStringToObject(error,
			// PROPERTY_NAME_MESSAGE, PROPERTY_NAME_UNDEFINED_MESSAGE);
			return response;
	}
	result = cJSON_AddObjectToObject(response, PROPERTY_NAME_RESULT);
	/* Let's finally assembly the response object. */
	cJSON_AddItemToObject(result, PROPERTY_NAME_VALUE, value);
	return response;
}
