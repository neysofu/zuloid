#include "chess/bb.h"
#include "chess/find_magics.h"
#include "debug.h"
#include "mt-64/mt-64.h"
#include "utils.h"
#include <assert.h>
#include <libpopcnt/libpopcnt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

Bitboard BB_ATTACKS_BISHOP[64 * 4096] = { 0 };
Bitboard BB_ATTACKS_ROOK[64 * 4096] = { 0 };

Bitboard BB_MASK_BISHOP[SQUARES_COUNT] = { 0 };
Bitboard BB_MASK_ROOK[SQUARES_COUNT] = { 0 };
Bitboard BB_POSTMASK_ROOK[SQUARES_COUNT] = { 0 };
Bitboard BB_POSTMASK_BISHOP[SQUARES_COUNT] = { 0 };

int64_t BB_MULTIPLIERS_BISHOP[64] = { 0 };
int64_t BB_MULTIPLIERS_ROOK[64] = { 0 };

size_t BB_OFFSETS_ROOK[64] = { 0 };
size_t BB_OFFSETS_BISHOP[64] = { 0 };

int64_t BB_SHIFTS_ROOK[64] = { 52, 52 };
int64_t BB_SHIFTS_BISHOP[64] = { 0 };

Bitboard
bb_bishop_magic(Square sq, Bitboard occupancy)
{
	Bitboard mask = BB_MASK_BISHOP[sq] & occupancy;
	size_t i = (mask * BB_MULTIPLIERS_BISHOP[sq]) >> BB_SHIFTS_BISHOP[sq];
	return BB_ATTACKS_BISHOP[i + BB_OFFSETS_BISHOP[sq]];
}

Bitboard
bb_rook_magic(Square sq, Bitboard occupancy)
{
	Bitboard mask = BB_MASK_ROOK[sq] & occupancy;
	size_t i = (mask * BB_MULTIPLIERS_ROOK[sq]) >> BB_SHIFTS_ROOK[sq];
	return BB_ATTACKS_ROOK[i + BB_OFFSETS_ROOK[sq]];
}

void
init_attack_table(Square sq,
                  const struct Magic *magic,
                  Bitboard attacks_table[],
                  Bitboard (*slider)(Square, Bitboard))
{

	Bitboard subset = 0;
	do {
		size_t i = (subset * magic->multiplier) >> magic->rshift;
		attacks_table[i] = slider(sq, subset);
	} while ((subset = bb_next_subset(magic->premask, subset)));
}

void
bb_init_bishop(void)
{
	size_t offset = 0;
	for (Square sq = 0; sq <= SQUARE_MAX; sq++) {
		init_attack_table(sq, MAGICS_BISHOP + sq, BB_ATTACKS_BISHOP + offset, bb_bishop);
		BB_OFFSETS_BISHOP[sq] = offset;
		BB_MASK_BISHOP[sq] = MAGICS_BISHOP[sq].premask;
		BB_SHIFTS_BISHOP[sq] = MAGICS_BISHOP[sq].rshift;
		BB_MULTIPLIERS_BISHOP[sq] = MAGICS_BISHOP[sq].multiplier;
		BB_POSTMASK_BISHOP[sq] = MAGICS_BISHOP[sq].postmask;
		offset += MAGICS_BISHOP[sq].end - MAGICS_BISHOP[sq].start + 3;
	}
}

void
bb_init_rook(void)
{
	size_t offset = 0;
	for (Square sq = 0; sq <= SQUARE_MAX; sq++) {
		init_attack_table(sq, MAGICS + sq, BB_ATTACKS_ROOK + offset, bb_rook);
		BB_OFFSETS_ROOK[sq] = offset;
		BB_MASK_ROOK[sq] = MAGICS[sq].premask;
		BB_SHIFTS_ROOK[sq] = MAGICS[sq].rshift;
		BB_MULTIPLIERS_ROOK[sq] = MAGICS[sq].multiplier;
		BB_POSTMASK_ROOK[sq] = MAGICS[sq].postmask;
		offset += MAGICS[sq].end - MAGICS[sq].start + 3;
	}
}

void
magic_init(void)
{
	static bool done = false;
	if (done) {
		return;
	}
	done = true;
	bb_init_rook();
	bb_init_bishop();
}

