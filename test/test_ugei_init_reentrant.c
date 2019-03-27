/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "Unity/src/unity.h"
#include "engine.h"
#include <string.h>

void
test_ugei_init_reentrant(void)
{
	struct Engine *engine = engine_new();
	engine_call(engine, "{\"method\":\"init\"}");
	//char *response = engine_call(engine, "{\"method\":\"init\",\"id\":null}");
	//TEST_ASSERT(response);
	//TEST_ASSERT_NULL(strstr(response, "error"));
	//free(response);
	free(engine);
}
