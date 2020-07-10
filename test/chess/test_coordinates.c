#include "Unity/src/unity.h"
#include "chess/coordinates.h"

void
test_file_to_char(void)
{
	TEST_ASSERT_EQUAL_INT('a', file_to_char(0));
	TEST_ASSERT_EQUAL_INT('b', file_to_char(1));
	TEST_ASSERT_EQUAL_INT('c', file_to_char(2));
	TEST_ASSERT_EQUAL_INT('d', file_to_char(3));
	TEST_ASSERT_EQUAL_INT('e', file_to_char(4));
	TEST_ASSERT_EQUAL_INT('f', file_to_char(5));
	TEST_ASSERT_EQUAL_INT('g', file_to_char(6));
	TEST_ASSERT_EQUAL_INT('h', file_to_char(7));
}

void
test_char_to_file(void)
{
	TEST_ASSERT_EQUAL_INT(0, char_to_file('a'));
	TEST_ASSERT_EQUAL_INT(1, char_to_file('b'));
	TEST_ASSERT_EQUAL_INT(2, char_to_file('c'));
	TEST_ASSERT_EQUAL_INT(3, char_to_file('d'));
	TEST_ASSERT_EQUAL_INT(4, char_to_file('e'));
	TEST_ASSERT_EQUAL_INT(5, char_to_file('f'));
	TEST_ASSERT_EQUAL_INT(6, char_to_file('g'));
	TEST_ASSERT_EQUAL_INT(7, char_to_file('h'));
}
