#include "chess/bb.h"
#include "chess/magic.h"
#include "chess/threats.h"
#include "libpopcnt/libpopcnt.h"
#include "mt-64/mt-64.h"
#include "munit/munit.h"
#include "utils.h"
#include <time.h>

void
test_magic_generation(void)
{
	srand(time(NULL));
	init_threats();
	for (size_t i = 0; i < 5000; i++) {
		Square from = rand() % 64;
		Bitboard mask = genrand64_int64() & genrand64_int64();
		munit_assert_uint(threats_by_rook(from, mask), ==, threats_by_rook_no_init(from, mask));
		munit_assert_uint(threats_by_bishop(from, mask), ==, threats_by_bishop_no_init(from, mask));
	}
}

void
test_bb_subset(void)
{
	Bitboard mask = 0x808080830080000ull;
	Bitboard subset = 0;
	size_t i = 0;
	while ((subset = bb_next_subset(mask, subset))) {
		i++;
	}
	munit_assert_uint(i + 1, ==, 1ULL << popcount64(mask));
}
