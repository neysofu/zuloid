/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "Unity/src/unity.h"
#include "engine.h"
#include "globals.h"
#include "utils.h"
#include <string.h>

void
test_ugei_config(void)
{
	struct Engine *engine = engine_new();
	engine_call(
	  engine, "{\"method\":\"config\",\"params\":{\"key\":\"contempt\",\"value\":0.1337}}");
	TEST_ASSERT_EQUAL_FLOAT(engine->contempt, 0.1337);
	engine_delete(engine);
}
