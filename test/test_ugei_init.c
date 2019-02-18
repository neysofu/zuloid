/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "Unity/src/unity.h"
#include "engine.h"
#include "globals.h"
#include <string.h>

void
test_ugei_init_metadata(void)
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
