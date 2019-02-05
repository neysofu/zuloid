/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "UGEI/property_names.h"
#include "cJSON/cJSON.h"
#include "engine.h"
#include "globals.h"

void
engine_call_init(struct Engine *engine, const struct cJSON *params, struct cJSON *response)
{
	struct cJSON *result = cJSON_AddObjectToObject(response, PROPERTY_NAME_RESULT);
	struct cJSON *meta = cJSON_AddObjectToObject(result, PROPERTY_NAME_META);
	cJSON_AddStringToObject(meta, PROPERTY_NAME_COPYRIGHT, Z64C_COPYRIGHT);
	cJSON_AddStringToObject(meta, PROPERTY_NAME_LICENSE, Z64C_LICENSE);
	cJSON_AddStringToObject(meta, PROPERTY_NAME_NAME, "Z64C");
	cJSON_AddStringToObject(meta, PROPERTY_NAME_RELEASE_DATE, Z64C_RELEASE_DATE_ISO_8601);
	cJSON_AddStringToObject(meta, PROPERTY_NAME_URL, Z64C_URL);
	cJSON_AddStringToObject(meta, PROPERTY_NAME_VERSION, Z64C_VERSION);
	struct cJSON *rulesets = cJSON_AddArrayToObject(result, PROPERTY_NAME_RULESETS);
	cJSON_AddItemToArray(rulesets, cJSON_CreateString("chess"));
}
