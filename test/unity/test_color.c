/* Copyright (c) Filippo Costa - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * PROPRIETARY AND CONFIDENTIAL */

#include "Unity/src/unity.h"
#include "chess/color.h"

void
test_color(void)
{
	TEST_ASSERT_EQUAL_INT(COLOR_BLACK, color_other(COLOR_WHITE));
	TEST_ASSERT_EQUAL_INT(COLOR_WHITE, color_other(COLOR_BLACK));
}
