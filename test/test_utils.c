#include "engine.h"
#include "munit/munit.h"
#include "protocols/uci.h"
#include "test/utils.h"
#include "utils.h"

void
test_utils(void)
{
	FILE *file = fopen(TEST_RESOURCES "lines/empty.txt", "r");
	struct Lines *lines = read_last_lines(file);
	munit_assert_uint(lines_count(lines), ==, 0);
	munit_assert_null(lines_nth(lines, -1));
	munit_assert_null(lines_nth(lines, -17));
	munit_assert_null(lines_nth(lines, 0));
	munit_assert_null(lines_nth(lines, 1));
	munit_assert_null(lines_nth(lines, 8));
	fclose(file);
}
