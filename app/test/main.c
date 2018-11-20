/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "Unity/src/unity.h"
#include "chess/board.h"
#include "chess/move.h"
#include "cJSON/cJSON.h"

extern void test_utils(void);

int
main(void)
{
    UNITY_BEGIN();
	//RUN_TEST(test_perft_all);
	RUN_TEST(test_utils);
    return UNITY_END();
}
