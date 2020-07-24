#include "chess/bb.h"
#include "chess/diagonals.h"
#include "munit/munit.h"
#include "utils.h"

enum
{
	DIAGONALS_COUNT = ARRAY_SIZE(DIAGONALS_A8H1),
};

void
test_diagonals_dont_overlap(const Bitboard diagonals[DIAGONALS_COUNT])
{
	Bitboard bb = 0;
	for (size_t i = 0; i < DIAGONALS_COUNT; i++) {
		munit_assert_uint64(bb & diagonals[i], ==, 0);
		bb |= diagonals[i];
	}
	munit_assert_uint64(bb, ==, UINT64_MAX);
}
