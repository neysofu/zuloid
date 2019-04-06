/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "Unity/src/unity.h"
#include "engine.h"
#include "globals.h"
#include <string.h>

#define FEN "R2K1B1R/PPP2QPP/5N1N/1n2q3/8/1p3n2/pbpp3p/1kr4r w KQkq - 0 1"

void
test_ugei_setup(void)
{
	struct Engine *engine = engine_new();
	engine_call(engine, "{\"method\":\"setup\",\"params\":{\"fen\":\"" FEN "\"}}");
	//char *response =
	//  engine_call(engine, "{\"method\":\"get\",\"params\":{\"key\":\"FEN\"},\"id\":0}");
	//TEST_ASSERT_TRUE(strstr(response, FEN));
	//free(response);
	free(engine);
}
