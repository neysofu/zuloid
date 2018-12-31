/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "Unity/src/unity.h"
#include "engine.h"
#include <string.h>

#define TEST_ASSERT_STRSTR(str, substr) TEST_ASSERT_NOT_NULL(strstr(str, substr))

void
test_jsonrpc_init(void)
{
	struct Engine *engine = engine_new();
	char *response;
	response = engine_call(engine, "{\"method\":\"init\",\"id\":0}");
	TEST_ASSERT_STRSTR(response, "meta");
	TEST_ASSERT_STRSTR(response, "Z64C");
	TEST_ASSERT_STRSTR(response, "meta");
	TEST_ASSERT_STRSTR(response, "chess");
	free(response);
	engine_delete(engine);
}

void
test_jsonrpc_status(void)
{
	struct Engine *engine = engine_new();
	char *response;
	response = engine_call(engine, "{\"method\":\"status\",\"params\":{\"foo\":\"bar\"},\"id:0\"}");
	TEST_ASSERT_NOT_NULL(response);
	free(response);
	engine_delete(engine);
}

void
test_jsonrpc_exit(void)
{
	struct Engine *engine = engine_new();
	engine_call(engine, "{\"method\":\"exit\"}");
	TEST_ASSERT_EQUAL_INT(engine->mode, MODE_EXIT);
	engine_delete(engine);
}

void
test_jsonrpc_set_then_get(void)
{
	struct Engine *engine = engine_new();
	char *response;
	engine_call(engine, "{\"method\":\"set\",\"params\":{\"key\":\"contempt\",\"value\":0.125}}");
	response = engine_call(engine, "{\"method\":\"get\",\"params\":{\"key\":\"contempt\"},\"id\":0}");
	TEST_ASSERT_STRSTR(response, "0.125");
	free(response);
	engine_call(engine, "{\"method\":\"set\",\"params\":{\"key\":\"selectivity\",\"value\":0.5555}}");
	response = engine_call(engine, "{\"method\":\"get\",\"params\":{\"key\":\"selectivity\"},\"id\":0}");
	TEST_ASSERT_STRSTR(response, "0.555");
	free(response);
	engine_delete(engine);
}

void
test_jsonrpc_setup(void)
{
	struct Engine *engine = engine_new();
	//engine_call(engine, "  {\"jsonrpc\":\"2.0\", \"method\":\"setup\",\"params\":{\"FEN\":\"" "1r2r1k1/p4ppp/1pn5/3Q4/8/5N1P/PP3PP1/R5K1 b - - 0 2"  "\"}\"id\":null}");
	char *response = engine_call(engine, "{\"method\":\"get\",\"params\":{\"key\":\"FOO\"},\"id\":0}");
	TEST_ASSERT_STRSTR(response, "1r2r1k1/p4ppp/1pn5/3Q4/8/5N1P/PP3PP1/R5K1 b - -");
	free(response);
	engine_delete(engine);
}
