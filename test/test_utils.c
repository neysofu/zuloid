#include "munit/munit.h"
#include "test/utils.h"
#include <limits.h>

void
test_utils(void)
{
	FILE *file = fopen(TEST_RESOURCES "lines/empty.txt", "r");
	{
		struct Lines *lines = file_line_by_line(file);
		munit_assert_uint(lines_count(lines), ==, 0);
		munit_assert_null(lines_nth(lines, -1));
		munit_assert_null(lines_nth(lines, -17));
		munit_assert_null(lines_nth(lines, 0));
		munit_assert_null(lines_nth(lines, 1));
		munit_assert_null(lines_nth(lines, 8));
		munit_assert_null(lines_nth(lines, INT_MAX));
		munit_assert_null(lines_nth(lines, INT_MIN));
		lines_delete(lines);
	}
	fclose(file);
}
