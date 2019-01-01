/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "Unity/src/unity.h"
#include "Z64C.h"

#define TEST_ASSERT_STRSTR(str, substr) TEST_ASSERT_NOT_NULL(strstr(str, substr))

void
test_jsonrpc_no_response_from_notification(void)
{
	struct Engine *engine = engine_new();
	TEST_ASSERT_NULL(engine_send_request(engine, "{\"method\":\"status\"}"));
	TEST_ASSERT_NULL(engine_send_request(engine, "{\"method\":\"init\",\"params\":[]}"));
	TEST_ASSERT_NULL(engine_send_request(engine, "{\"jsonrpc\":\"2.0\",\"method\":\"foobar\"}"));
	engine_delete(engine);
}

void
test_jsonrpc_no_response_from_empty_request(void)
{
	struct Engine *engine = engine_new();
	TEST_ASSERT_NULL(engine_send_request(engine, " \t # comment!"));
	TEST_ASSERT_NULL(engine_send_request(engine, ""));
	TEST_ASSERT_NULL(engine_send_request(engine, "\r\n"));
	engine_delete(engine);
}

void
test_jsonrpc_non_null_response(void)
{
	struct Engine *engine = engine_new();
	char *response;
	response = engine_send_request(engine, "foobar");
	TEST_ASSERT_NOT_NULL(response);
	free(response);
	response = engine_send_request(engine, "{}");
	TEST_ASSERT_NOT_NULL(response);
	free(response);
	response = engine_send_request(engine, "{\"method\":\"init\",\"id\":null}");
	TEST_ASSERT_NOT_NULL(response);
	free(response);
	response = engine_send_request(engine, "{\"method\":\"foobar\",\"id\":1337}");
	TEST_ASSERT_NOT_NULL(response);
	free(response);
	response = engine_send_request(engine, "{\"foo\":\"foobar\",\"id\":null}");
	TEST_ASSERT_NOT_NULL(response);
	free(response);
	engine_delete(engine);
}
