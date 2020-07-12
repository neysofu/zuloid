#include "chess/pieces.h"
#include "munit/munit.h"

void
test_piece_to_char(void)
{
	struct Piece piece = { .type = PIECE_TYPE_QUEEN, .color = COLOR_WHITE };
	munit_assert_char('Q', ==, piece_to_char(piece));
	piece = PIECE_NONE;
	munit_assert_char('.', ==, piece_to_char(piece));
	piece = (struct Piece){ .type = PIECE_TYPE_ROOK, .color = COLOR_BLACK };
	munit_assert_char('r', ==, piece_to_char(piece));
	piece = (struct Piece){ .type = PIECE_TYPE_PAWN, .color = COLOR_WHITE };
	munit_assert_char('P', ==, piece_to_char(piece));
}
