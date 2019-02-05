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

void
engine_dispatch_call(struct Engine *engine,
                     const char *method,
                     const struct cJSON *params,
                     struct cJSON *response)
{
	switch (XXH64(method, strlen(method), 0)) {
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
		case 0x71e6f6d1e157dbfe: /* "search" */
			engine_call_search(engine, params, response);
			break;
		case 0xde2128284d8fdb3c: /* "setup" */
			engine_call_setup(engine, params, response);
			break;
		case 0xc641b2419f8a3ce1: /* "status" */
			break;
		default:
			cJSON_AddJsonRpcErrorToObject(response, JSONRPC_INVALID_METHOD);
	}
}

char *
engine_send_request(struct Engine *engine, const char *str)
{
	if (!str || string_is_whitespace(str)) {
		return NULL;
	}
	struct cJSON *request = cJSON_Parse(str);
	struct cJSON *method = cJSON_GetObjectItem(request, PROPERTY_NAME_METHOD);
	struct cJSON *params = cJSON_GetObjectItem(request, PROPERTY_NAME_PARAMS);
	struct cJSON *id = cJSON_DetachItemFromObject(request, PROPERTY_NAME_ID);
	bool is_valid_request = cJSON_IsString(method);
	struct cJSON *response = is_valid_request && !id ? NULL : cJSON_CreateObject();
	if (is_valid_request) {
		engine_dispatch_call(engine, method->valuestring, params, response);
	} else if (request) {
		id = id ? id : cJSON_CreateNull();
		cJSON_AddJsonRpcErrorToObject(response, JSONRPC_INVALID_REQUEST);
	} else {
		id = cJSON_CreateNull();
		cJSON_AddJsonRpcErrorToObject(response, JSONRPC_PARSE_ERROR);
	}
	cJSON_Delete(request);
	cJSON_AddItemToObject(response, PROPERTY_NAME_ID, id);
	cJSON_AddStringToObject(response, "jsonrpc", "2.0");
	char *response_string = cJSON_PrintUnformatted(response);
	cJSON_Delete(response);
	return response_string;
}
