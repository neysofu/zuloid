/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "cJSON/cJSON.h"
#include "engine.h"
#include "globals.h"
#include <assert.h>

void
engine_call_init(struct Engine *engine, const cJSON *params, cJSON *response)
{
	cJSON *result = cJSON_AddObjectToObject(response, "result");
	cJSON *meta = cJSON_AddObjectToObject(result, "meta");
	cJSON_AddStringToObject(meta, "copyright", Z64C_COPYRIGHT);
	cJSON_AddStringToObject(meta, "license", Z64C_LICENSE);
	cJSON_AddStringToObject(meta, "name", "Z64C");
	cJSON_AddStringToObject(meta, "release_date", Z64C_RELEASE_DATE_ISO_8601);
	cJSON_AddStringToObject(meta, "url", Z64C_URL);
	cJSON_AddStringToObject(meta, "version", Z64C_VERSION);
	cJSON *rulesets = cJSON_AddArrayToObject(result, "rulesets");
	cJSON_AddItemToArray(rulesets, cJSON_CreateString("chess"));
}
