/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "UGEI/errors.h"
#include "UGEI/methods.h"
#include "UGEI/property_names.h"
#include "cJSON/cJSON.h"
#include "engine.h"
#include "utils.h"
#include "xxHash/xxhash.h"
#include <stdio.h>
#include <string.h>

char *
engine_send_request(struct Engine *engine, const char *str)
{
	struct cJSON *request = cJSON_Parse(str);
	struct cJSON *method = cJSON_GetObjectItem(request, PROPERTY_NAME_METHOD);
	struct cJSON *params = cJSON_GetObjectItem(request, PROPERTY_NAME_PARAMS);
	struct cJSON *id = cJSON_DetachItemFromObject(request, PROPERTY_NAME_ID);
	struct cJSON *response = !id && method ? NULL : cJSON_CreateObject();
	if (!method) {
		if (string_is_whitespace(str)) {
			return NULL;
		}
		id = cJSON_CreateNull();
		cJSON_AddJsonRpcErrorToObject(
		  response, request ? JSONRPC_INVALID_REQUEST : JSONRPC_PARSE_ERROR);
		goto finalize_response_and_clean_up;
	} else if (!cJSON_IsString(method)) {
		goto deal_with_invalid_method;
	}
	switch (XXH64(method->valuestring, strlen(method->valuestring), 0)) {
		case 0x3e6da0adb9a81aa0: /* "exit" */
			engine->mode = MODE_EXIT;
			break;
		case 0x2682377a1193c238: /* "get" */
			engine_call_get(engine, params, response);
			break;
		case 0xbbb05418c88423aa: /* "init" */
			engine_call_init(engine, params, response);
			break;
		case 0x284938c798d362a8: /* "config" */
			engine_call_config(engine, params, response);
			break;
		case 0xde2128284d8fdb3c: /* "setup" */
			engine_call_setup(engine, params, response);
			break;
		case 0xc641b2419f8a3ce1: /* "status" */
			break;
		deal_with_invalid_method:
		default:
			cJSON_AddJsonRpcErrorToObject(response, JSONRPC_INVALID_METHOD);
	}
finalize_response_and_clean_up:
	cJSON_Delete(request);
	cJSON_AddItemToObject(response, PROPERTY_NAME_ID, id);
	cJSON_AddStringToObject(response, "jsonrpc", "2.0");
	char *response_string = cJSON_PrintUnformatted(response);
	cJSON_Delete(response);
	return response_string;
}
