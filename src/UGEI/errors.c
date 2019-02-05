/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "UGEI/errors.h"
#include "UGEI/property_names.h"
#include "cJSON/cJSON.h"

const char *
jsonrpc_error_default_message(enum JsonRpcError error)
{
	switch (error) {
		case JSONRPC_PARSE_ERROR:
			return "Parse error";
		case JSONRPC_INVALID_PARAMS:
			return "Invalid params";
		case JSONRPC_INVALID_METHOD:
			return "Invalid method";
		case JSONRPC_INVALID_REQUEST:
			return "Invalid Request";
		case JSONRPC_GENERIC_ERROR:
			return "Generic error";
		case JSONRPC_UNSUPPORTED_RULESET:
			return "Unsupported ruleset";
		case JSONRPC_UNDEFINED_KEY:
			return "Undefined key";
		default:
			return "Generic error";
	}
}

CJSON_PUBLIC(cJSON *)
cJSON_AddJsonRpcErrorToObject(cJSON *object, enum JsonRpcError error)
{
	if (!object) {
		return NULL;
	}
	const char *message = jsonrpc_error_default_message(error);
	struct cJSON *message_item = cJSON_CreateStringReference(message);
	struct cJSON *error_item = cJSON_AddObjectToObject(object, PROPERTY_NAME_ERROR);
	cJSON_AddNumberToObject(error_item, PROPERTY_NAME_CODE, error);
	cJSON_AddItemToObject(error_item, PROPERTY_NAME_MESSAGE, message_item);
	return error_item;
}
