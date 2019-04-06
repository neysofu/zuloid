/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "api/jsonrpc_errors.h"
#include "cJSON/cJSON.h"
#include "engine.h"
#include "globals.h"
#include "utils/utils.h"

void
engine_ugei_call_init(struct Engine *engine, cJSON *response)
{
	engine->agent = agent_new();
	engine->time_controls[COLOR_WHITE] = time_control_new_bullet();
	game_clock_init(&engine->game_clocks[COLOR_WHITE], engine->time_controls[COLOR_WHITE]);
	game_clock_init(&engine->game_clocks[COLOR_BLACK], engine->time_controls[COLOR_WHITE]);
	if (!engine->agent) {
		cJSON_AddJsonRpcErrorToObject(response, JSONRPC_GENERIC_ERROR);
		return;
	}
	cJSON *result = cJSON_AddObjectToObject(response, "result");
	cJSON *meta = cJSON_AddObjectToObject(result, "meta");
	cJSON_AddStringToObject(meta, "copyright", Z64C_COPYRIGHT);
	cJSON_AddStringToObject(meta, "license", Z64C_LICENSE);
	cJSON_AddStringToObject(meta, "name", "Z64C");
	cJSON_AddStringToObject(meta, "release_date", Z64C_BUILD_DATE);
	cJSON_AddStringToObject(meta, "url", Z64C_URL);
	cJSON_AddStringToObject(meta, "version", Z64C_VERSION);
	cJSON *rulesets = cJSON_AddArrayToObject(result, "rulesets");
	cJSON_AddItemToArray(rulesets, cJSON_CreateString("chess"));
}
