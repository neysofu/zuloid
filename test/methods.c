/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "Unity/src/unity.h"
#include "engine.h"
#include "globals.h"
#include "utils.h"
#include <string.h>

void
test_method_get_settings(void)
{
	struct Engine *engine = engine_new();
	engine_call(
	  engine, "{\"method\":\"config\",\"params\":{\"key\":\"contempt\",\"value\":0.8512}}");
	char *response = engine_call(
	  engine, "{\"method\":\"get\",\"params\":{\"key\":\"contempt\"},\"id\":0}");
	TEST_ASSERT(strstr(response, "0.851"));
	free(response);
	free(engine);
}

void
test_method_init_metadata(void)
{
	struct Engine *engine = engine_new();
	char *response = engine_call(engine, "{\"method\":\"init\",\"id\":null}");
	TEST_ASSERT(response);
	TEST_ASSERT(strstr(response, "meta"));
	TEST_ASSERT(strstr(response, Z64C_COPYRIGHT));
	TEST_ASSERT(strstr(response, Z64C_VERSION));
	free(response);
	free(engine);
}

void
test_method_init_reentrancy(void)
{
	struct Engine *engine = engine_new();
	engine_call(engine, "{\"method\":\"init\"}");
	char *response = engine_call(engine, "{\"method\":\"init\",\"id\":null}");
	TEST_ASSERT(response);
	TEST_ASSERT_NULL(strstr(response, "error"));
	free(response);
	free(engine);
}

void
test_method_setup_basic_fen(void)
{
#define FEN "R2K1B1R/PPP2QPP/5N1N/1n2q3/8/1p3n2/pbpp3p/1kr4r w KQkq - 0 1"
	struct Engine *engine = engine_new();
	engine_call(engine, "{\"method\":\"setup\",\"params\":{\"fen\":\"" FEN "\"}}");
	char *response =
	  engine_call(engine, "{\"method\":\"get\",\"params\":{\"key\":\"FEN\"},\"id\":0}");
	TEST_ASSERT_TRUE(strstr(response, FEN));
	free(response);
	free(engine);
}
