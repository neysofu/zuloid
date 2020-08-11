#include "chess/bb.h"
#include "chess/bb_subset_iter.h"
#include "chess/find_magics.h"
#include "libpopcnt/libpopcnt.h"
#include "mt-64/mt-64.h"
#include "munit/munit.h"
#include "utils.h"
#include <time.h>

extern Bitboard
bb_rook_magic(Square sq, Bitboard occupancy);
extern Bitboard
bb_bishop_magic(Square sq, Bitboard occupancy);

void
test_magic_generation(void)
{
	srand(time(NULL));
	bb_init();
	for (size_t i = 0; i < 5000; i++) {
		Square from = rand() % 64;
		Bitboard mask = genrand64_int64() & genrand64_int64();
		munit_assert_uint(bb_rook(from, mask), ==, bb_rook_magic(from, mask));
		munit_assert_uint(bb_bishop(from, mask), ==, bb_bishop_magic(from, mask));
	}
}

void
test_bb_subset(void)
{
	struct BitboardSubsetIter iter = bb_subset_iter_from_mask(0x808080830080000ull);
	size_t i = 0;
	while (bb_subset_iter(&iter)) {
		i++;
	}
	munit_assert_uint(i + 1, ==, 1ULL << popcount64(iter.original));
}
