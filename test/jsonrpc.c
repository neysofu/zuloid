/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "UGEI/property_names.h"
#include "Unity/src/unity.h"
#include "engine.h"
#include <stdlib.h>
#include <string.h>

void
test_jsonrpc_error_response_to_unparsable_request(void)
{
	struct Engine *engine = engine_new();
	char *response = engine_call(engine, "spam");
	TEST_ASSERT(response && strstr(response, PROPERTY_NAME_ERROR));
	free(response);
	free(engine);
}

void
test_jsonrpc_error_response_to_invalid_request(void)
{
	struct Engine *engine = engine_new();
	char *response = engine_call(engine, "{}");
	TEST_ASSERT(response && strstr(response, PROPERTY_NAME_ERROR));
	free(response);
	free(engine);
}

void
test_jsonrpc_response_with_id_to_invalid_request_with_id(void)
{
	struct Engine *engine = engine_new();
	char *response = engine_call(engine, "{\"id\":42}");
	TEST_ASSERT(response && strstr(response, "42"));
	free(response);
	free(engine);
}

void
test_jsonrpc_no_response_to_invalid_request_with_no_id(void)
{
	struct Engine *engine = engine_new();
	TEST_ASSERT_NULL(engine_call(engine, "{\"method\":\"abcxyz\",\"params\":[]}"));
	free(engine);
}

void
test_jsonrpc_no_response_to_empty_request(void)
{
	struct Engine *engine = engine_new();
	TEST_ASSERT_NULL(engine_call(engine, ""));
	TEST_ASSERT_NULL(engine_call(engine, "\t \r\n"));
	free(engine);
}
