/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "cJSON/cJSON.h"
#include "engine.h"

void
engine_call_search(struct Engine *engine, const cJSON *params, cJSON *response)
{
	cJSON *result = cJSON_AddObjectToObject(response, "result");
	cJSON *evaluation = cJSON_AddNumberToObject(result, "evaluation", 0);
}
