/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "UI/jsonrpc_errors.h"
#include "cJSON/cJSON.h"
#include "chess/fen.h"
#include "chess/position.h"
#include "engine.h"
#include "globals.h"
#include "utils.h"
#include "xxHash/xxhash.h"
#include <string.h>

void
engine_ugei_call_get(struct Engine *engine, const cJSON *params, cJSON *response)
{
	cJSON *value;
	cJSON *key = cJSON_GetObjectItem(params, "key");
	if (!cJSON_IsObject(params) || !cJSON_IsString(key)) {
		cJSON_AddJsonRpcErrorToObject(response, JSONRPC_INVALID_PARAMS);
		return;
	}
	switch (XXH64(key->valuestring, strlen(key->valuestring), 0)) {
		case 0x63f596aaf1c3bed7: /* FEN */
			value = cJSON_CreateString(fen_new_from_position(&engine->position));
			break;
		case 0x46f9bde5468e310a: /* "move_selection_noise" */
			value = cJSON_CreateNumber(engine->move_selection_noise);
			break;
		case 0x597a99f05012dd35: /* "contempt" */
			value = cJSON_CreateNumber(engine->contempt);
			break;
		case 0x92cfb5ce943dee0d: /* "selectivity" */
			value = cJSON_CreateNumber(engine->selectivity);
			break;
		default:
			cJSON_AddJsonRpcErrorToObject(response, JSONRPC_UNDEFINED_KEY);
			return;
	}
	cJSON *result = cJSON_AddObjectToObject(response, "result");
	cJSON_AddItemToObject(result, "value", value);
	return;
}
