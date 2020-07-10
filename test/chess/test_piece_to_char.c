#include "Unity/src/unity.h"
#include "chess/pieces.h"

void
test_piece_to_char(void)
{
	struct Piece piece = { .type = PIECE_TYPE_QUEEN, .color = COLOR_WHITE };
	TEST_ASSERT_EQUAL_INT('Q', piece_to_char(piece));
	piece = PIECE_NONE;
	TEST_ASSERT_EQUAL_INT('.', piece_to_char(piece));
	piece = (struct Piece){ .type = PIECE_TYPE_ROOK, .color = COLOR_BLACK };
	TEST_ASSERT_EQUAL_INT('r', piece_to_char(piece));
	piece = (struct Piece){ .type = PIECE_TYPE_PAWN, .color = COLOR_WHITE };
	TEST_ASSERT_EQUAL_INT('P', piece_to_char(piece));
}
