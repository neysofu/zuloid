#include "Unity/src/unity.h"
#include "chess/pieces.h"

void
test_char_to_piece(void)
{
	TEST_ASSERT_EQUAL_INT(PIECE_TYPE_QUEEN, char_to_piece('Q').type);
	TEST_ASSERT_EQUAL_INT(PIECE_TYPE_QUEEN, char_to_piece('q').type);
	TEST_ASSERT_EQUAL_INT(PIECE_TYPE_KING, char_to_piece('k').type);
	TEST_ASSERT_EQUAL_INT(PIECE_TYPE_BISHOP, char_to_piece('B').type);
	TEST_ASSERT_EQUAL_INT(PIECE_TYPE_NONE, char_to_piece('.').type);
	TEST_ASSERT_EQUAL_INT(PIECE_TYPE_NONE, char_to_piece('?').type);
	TEST_ASSERT_EQUAL_INT(COLOR_WHITE, char_to_piece('B').color);
	TEST_ASSERT_EQUAL_INT(COLOR_BLACK, char_to_piece('q').color);
}
