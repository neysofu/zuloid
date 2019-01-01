/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "UGEI/errors.h"
#include "UGEI/property_names.h"
#include "cJSON/cJSON.h"

struct cJSON *
cJSON_CreateJsonRpcError(enum JsonRpcError err)
{
	struct cJSON *error = cJSON_CreateObject();
	cJSON_AddNumberToObject(error, PROPERTY_NAME_CODE, err);
	char *msg = "";
	switch (err) {
		/* TODO: handle all cases. */
		case JSONRPC_PARSE_ERROR:
			msg = "Parse error";
			break;
		case JSONRPC_INVALID_PARAMS:
			msg = "Invalid params";
			break;
		case JSONRPC_INVALID_METHOD:
			msg = "Invalid method";
			break;
		case JSONRPC_INVALID_REQUEST:
			msg = "Invalid Request";
			break;
		case JSONRPC_GENERIC_ERROR:
			msg = "Generic error";
			break;
		case JSONRPC_UNSUPPORTED_RULESET:
			msg = "Unsupported ruleset";
			break;
		case JSONRPC_UNDEFINED_KEY:
			msg = "Undefined key";
			break;
	}
	cJSON_AddItemToObject(error, PROPERTY_NAME_MESSAGE, cJSON_CreateStringReference(msg));
	return error;
}
