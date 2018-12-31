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
engine_call(struct Engine *engine, const char *str)
{
	if (string_is_comment_or_whitespace(str)) {
		return NULL;
	}
	struct cJSON *request = cJSON_Parse(str);
	struct cJSON *response;
	struct cJSON *id;
	/* Be aware of threading issues with cJSON_GetErrorPtr. No problem here
	 * because it is single-threaded. */
	if (!request || cJSON_GetErrorPtr()) {
		response = cJSON_CreateObject();
		cJSON_AddItemToObject(response,
		                      PROPERTY_NAME_ERROR,
		                      cJSON_CreateJsonRpcError(JSONRPC_PARSE_ERROR));
		id = cJSON_CreateNull();
		goto respond;
	}
	const struct cJSON *method = cJSON_GetObjectItem(request, PROPERTY_NAME_METHOD);
	if (!cJSON_IsString(method)) {
		response = cJSON_CreateObject();
		id = cJSON_CreateNull();
		cJSON_AddItemToObject(
		  response,
		  PROPERTY_NAME_ERROR,
		  cJSON_CreateJsonRpcError(JSONRPC_INVALID_REQUEST));
		goto respond;
	}
	const struct cJSON *params = cJSON_GetObjectItem(request, PROPERTY_NAME_PARAMS);
	id = cJSON_GetObjectItem(request, PROPERTY_NAME_ID);
	cJSON_DetachItemViaPointer(request, id);
	switch (XXH64(method->valuestring, strlen(method->valuestring), 0)) {
		case 0x3e6da0adb9a81aa0: /* "exit" */
			engine->mode = MODE_EXIT;
			response = cJSON_CreateObject();
			break;
		case 0x2682377a1193c238: /* "get" */
			response = engine_call_get(engine, params);
			break;
		case 0xbbb05418c88423aa: /* "init" */
			response = engine_call_init(engine, params);
			break;
		case 0x284938c798d362a8: /* "config" */
			response = engine_call_config(engine, params);
			break;
		case 0xde2128284d8fdb3c: /* "setup" */
			response = engine_call_setup(engine, params);
			break;
		case 0xc641b2419f8a3ce1: /* "status" */
			response = cJSON_CreateObject();
			break;
		default:
			response = cJSON_CreateObject();
			cJSON_AddItemToObject(
			  response,
			  PROPERTY_NAME_ERROR,
			  cJSON_CreateJsonRpcError(JSONRPC_INVALID_METHOD));
	}
respond:
	cJSON_Delete(request);
	char *minified_response = NULL;
	if (id) {
		cJSON_AddStringToObject(response, "jsonrpc", "2.0");
		cJSON_AddItemToObject(response, PROPERTY_NAME_ID, id);
		minified_response = cJSON_PrintUnformatted(response);
	}
	cJSON_Delete(response);
	return minified_response;
}
