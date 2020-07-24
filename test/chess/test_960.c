#include "chess/bb.h"
#include "chess/movegen.h"
#include "chess/position.h"
#include "munit/munit.h"

enum
{
	NUMBER_OF_TESTS = 500,
};

void
test_960(void)
{
	for (size_t i = 0; i < NUMBER_OF_TESTS; i++) {
		struct Board position = POSITION_INIT;
		position_init_960(&position);
		Bitboard rooks = position.bb[PIECE_TYPE_ROOK];
		Bitboard kings = position.bb[PIECE_TYPE_KING];
		Bitboard bishops_on_white_squares =
		  position.bb[PIECE_TYPE_BISHOP] & 0x5555555555555555;
		Bitboard bishops_on_black_squares =
		  position.bb[PIECE_TYPE_BISHOP] & 0xaaaaaaaaaaaaaaaa;
		Bitboard occupancy = position.bb[COLOR_WHITE] | position.bb[COLOR_BLACK];
		Bitboard expected_occupancy =
		  POSITION_INIT.bb[COLOR_WHITE] | POSITION_INIT.bb[COLOR_BLACK];
		munit_assert_uint64(rooks, >, kings);
		munit_assert_uint64(bishops_on_black_squares, !=, 0);
		munit_assert_uint64(bishops_on_white_squares, !=, 0);
		munit_assert_uint64(occupancy, ==, expected_occupancy);
	}
}
