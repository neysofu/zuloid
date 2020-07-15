#include "chess/pieces.h"
#include "munit/munit.h"

void
test_char_to_piece(void)
{
	munit_assert_int(PIECE_TYPE_QUEEN, ==, char_to_piece('Q').type);
	munit_assert_int(PIECE_TYPE_QUEEN, ==, char_to_piece('q').type);
	munit_assert_int(PIECE_TYPE_KING, ==, char_to_piece('k').type);
	munit_assert_int(PIECE_TYPE_BISHOP, ==, char_to_piece('B').type);
	munit_assert_int(PIECE_TYPE_NONE, ==, char_to_piece('.').type);
	munit_assert_int(PIECE_TYPE_NONE, ==, char_to_piece('?').type);
	munit_assert_int(COLOR_WHITE, ==, char_to_piece('B').color);
	munit_assert_int(COLOR_BLACK, ==, char_to_piece('q').color);
}
