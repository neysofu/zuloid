/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "UGEI/property_names.h"
#include "Unity/src/unity.h"
#include "Z64C.h"
#include "globals.h"
#include <string.h>

void
test_method_init_metadata(void)
{
	struct Engine *engine = engine_new();
	char *response = engine_send_request(engine, "{\"method\":\"init\",\"id\":null}");
	TEST_ASSERT(response);
	TEST_ASSERT(strstr(response, PROPERTY_NAME_META));
	TEST_ASSERT(strstr(response, Z64C_COPYRIGHT));
	TEST_ASSERT(strstr(response, Z64C_VERSION));
	free(response);
	free(engine);
}

void
test_method_init_reentrancy(void)
{
	struct Engine *engine = engine_new();
	engine_send_request(engine, "{\"method\":\"init\"}");
	char *response = engine_send_request(engine, "{\"method\":\"init\",\"id\":null}");
	TEST_ASSERT(response);
	TEST_ASSERT_NULL(strstr(response, PROPERTY_NAME_ERROR));
	free(response);
	free(engine);
}

void
test_method_exit(void)
{
	struct Engine *engine = engine_new();
	engine_send_request(engine, "{\"method\":\"exit\"}");
	const int *exit_status = engine_exit_status(engine);
	TEST_ASSERT_NOT_NULL(exit_status);
	TEST_ASSERT_EQUAL_INT(*exit_status, EXIT_SUCCESS);
	free(engine);
}
