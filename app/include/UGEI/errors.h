/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_UGEI_ERRORS_H
#define Z64C_UGEI_ERRORS_H

#include "cJSON/cJSON.h"

enum JsonRpcError
{
	/* Predefined. */
	JSONRPC_PARSE_ERROR = -32700,
	JSONRPC_INVALID_PARAMS = -32602,
	JSONRPC_INVALID_METHOD = -32601,
	JSONRPC_INVALID_REQUEST = -32600,
	/* UGEI-specific. */
	JSONRPC_GENERIC_ERROR = 0,
	JSONRPC_UNSUPPORTED_RULESET = 900,
	JSONRPC_UNDEFINED_KEY = 902,
};

CJSON_PUBLIC(cJSON *)
cJSON_AddJsonRpcErrorToObject(cJSON *object, enum JsonRpcError);

#endif
