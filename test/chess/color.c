#include "Unity/src/unity.h"
#include "chess/color.h"

void
test_color_other(void)
{
	TEST_ASSERT_EQUAL_INT(COLOR_BLACK, color_other(COLOR_WHITE));
	TEST_ASSERT_EQUAL_INT(COLOR_WHITE, color_other(COLOR_BLACK));
}
