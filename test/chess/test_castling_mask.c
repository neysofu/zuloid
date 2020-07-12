#include "chess/fen.h"
#include "chess/position.h"
#include "munit/munit.h"

void
test_castling_mask(void)
{
	struct Board pos = POSITION_INIT;
	munit_assert_int64(
	  0x1010100000000, ==, position_castle_mask(&pos, CASTLING_RIGHT_KINGSIDE));
	munit_assert_int64(
	  0x101010000, ==, position_castle_mask(&pos, CASTLING_RIGHT_QUEENSIDE));
	char fen[FEN_SIZE] = "rbbqnkrn/pppppppp/8/8/8/8/PPPPPPPP/RBBQNKRN b KQkq - 0 1";
	position_init_from_fen(&pos, fen);
	munit_assert_int64(
	  0x80800000000000, ==, position_castle_mask(&pos, CASTLING_RIGHT_KINGSIDE));
	munit_assert_int64(
	  0x808080800000, ==, position_castle_mask(&pos, CASTLING_RIGHT_QUEENSIDE));
}
