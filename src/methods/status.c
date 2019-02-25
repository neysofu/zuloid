/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "cJSON/cJSON.h"
#include "engine.h"

void
engine_call_status(struct Engine *engine, const cJSON *params, cJSON *response)
{
	char *mode;
	switch (engine->mode) {
		case MODE_IDLE:
			mode = "idle";
		case MODE_SEARCH:
			mode = "searching";
		default:
			mode = "?";
	}
	cJSON *result = cJSON_AddObjectToObject(response, "result");
	cJSON_AddStringToObject(result, "mode", mode);
}
