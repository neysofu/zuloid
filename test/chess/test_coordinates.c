#include "chess/coordinates.h"
#include "munit/munit.h"

void
test_file_to_char(void)
{
	munit_assert_char('a', ==, file_to_char(0));
	munit_assert_char('b', ==, file_to_char(1));
	munit_assert_char('c', ==, file_to_char(2));
	munit_assert_char('d', ==, file_to_char(3));
	munit_assert_char('e', ==, file_to_char(4));
	munit_assert_char('f', ==, file_to_char(5));
	munit_assert_char('g', ==, file_to_char(6));
	munit_assert_char('h', ==, file_to_char(7));
}

void
test_char_to_file(void)
{
	munit_assert_int(0, ==, char_to_file('a'));
	munit_assert_int(1, ==, char_to_file('b'));
	munit_assert_int(2, ==, char_to_file('c'));
	munit_assert_int(3, ==, char_to_file('d'));
	munit_assert_int(4, ==, char_to_file('e'));
	munit_assert_int(5, ==, char_to_file('f'));
	munit_assert_int(6, ==, char_to_file('g'));
	munit_assert_int(7, ==, char_to_file('h'));
}
