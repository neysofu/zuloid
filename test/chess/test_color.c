#include "chess/color.h"
#include "munit/munit.h"

void
test_color_other(void)
{
	munit_assert_int(COLOR_BLACK, ==, color_other(COLOR_WHITE));
	munit_assert_int(COLOR_WHITE, ==, color_other(COLOR_BLACK));
}
