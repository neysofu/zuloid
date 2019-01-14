/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chess/color.h"
#include "Unity/src/unity.h"

void
test_color_other(void)
{
	TEST_ASSERT_EQUAL_INT(COLOR_BLACK, color_other(COLOR_WHITE));
	TEST_ASSERT_EQUAL_INT(COLOR_WHITE, color_other(COLOR_BLACK));
}
