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

Bitboard BB_ATTACKS_BY_KNIGHT[SQUARES_COUNT] = { 0 };
Bitboard BB_ATTACKS_BY_KING[SQUARES_COUNT] = { 0 };

Bitboard
slider_attacks(Square sq, Bitboard occupancy, const short delta[4][2])
{
	Bitboard bb = 0;
	for (int i = 0; i < 4; i++) {
		const int df = delta[i][0];
		const int dr = delta[i][1];
		File file = square_file(sq) + df;
		Rank rank = square_rank(sq) + dr;
		while (file >= 0 && file < 8 && rank >= 0 && rank < 8) {
			bb |= square_to_bb(square_new(file, rank));
			if (square_to_bb(square_new(file, rank)) & occupancy) {
				break;
			}
			rank += dr;
			file += df;
		}
	}
	return bb;
}

// Keep in mind that both knights and kings can move in 8 different directions.
const short OFFSETS_KNIGHT[8][2] = {
	{ -2, -1 }, { -2, 1 }, { 2, -1 }, { 2, 1 }, { -1, -2 }, { -1, 2 }, { 1, -2 }, { 1, 2 },
};

const short OFFSETS_KING[8][2] = {
	{ -1, -1 }, { 0, -1 }, { 1, -1 }, { -1, 0 }, { 1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 },
};

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

Bitboard BB_MAGICS_ROOK[SQUARES_COUNT] = { 0 };
Bitboard BB_MAGICS_BISHOP[SQUARES_COUNT] = { 0 };

void
bb_pprint(Bitboard bb)
{
	for (Rank rank = RANK_MAX; rank >= 0; rank--) {
		for (File file = 0; file <= FILE_MAX; file++) {
			putchar(bb & square_to_bb(square_new(file, rank)) ? 'x' : '.');
			putchar(' ');
		}
		puts("");
	}
	puts("");
}

Bitboard
bb_random(void)
{
	Bitboard u1, u2, u3, u4;
	u1 = (Bitboard)(random()) & 0xFFFF;
	u2 = (Bitboard)(random()) & 0xFFFF;
	u3 = (Bitboard)(random()) & 0xFFFF;
	u4 = (Bitboard)(random()) & 0xFFFF;
	return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}

Bitboard
bb_bishop(Square sq, Bitboard occupancy)
{
	const short bishop_offsets[4][2] = { { -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 1 } };
	return slider_attacks(sq, occupancy, bishop_offsets);
}

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

Bitboard
bb_rook(Square sq, Bitboard occupancy)
{
	const short rook_offsets[4][2] = { { -1, 0 }, { 0, -1 }, { 0, 1 }, { 1, 0 } };
	return slider_attacks(sq, occupancy, rook_offsets);
}

Bitboard
bb_attacks_by_offsets(Square sq, const short offsets[8][2])
{
	Bitboard bb = 0;
	for (size_t i = 0; i < 8; i++) {
		File f = square_file(sq) + offsets[i][0];
		Rank r = square_rank(sq) + offsets[i][1];
		if (r >= 0 && f >= 0 && r <= RANK_MAX && f <= FILE_MAX) {
			bb |= square_to_bb(square_new(f, r));
		}
	}
	return bb;
}

Bitboard
bb_rays_rook(Square sq)
{
	Bitboard x = rank_to_bb(square_rank(sq)) & ~file_to_bb(0) & ~file_to_bb(7);
	Bitboard y = file_to_bb(square_file(sq)) & ~rank_to_bb(0) & ~rank_to_bb(7);
	return x ^ y;
}

void
bb_init_bishop(void)
{
	size_t offset = 0;
	for (Square sq = 0; sq <= SQUARE_MAX; sq++) {
		magic_find_bishop(MAGICS_BISHOP + sq, sq, BB_ATTACKS_BISHOP + offset);
		BB_OFFSETS_BISHOP[sq] = offset;
		BB_MASK_BISHOP[sq] = MAGICS_BISHOP[sq].premask;
		BB_SHIFTS_BISHOP[sq] = MAGICS_BISHOP[sq].rshift;
		BB_MULTIPLIERS_BISHOP[sq] = MAGICS_BISHOP[sq].multiplier;
		BB_POSTMASK_BISHOP[sq] = MAGICS_BISHOP[sq].postmask;
		offset += MAGICS_BISHOP[sq].end - MAGICS_BISHOP[sq].start + 3;
	}
}

void
init_rook_attack_table(Square sq, const struct Magic *magic, Bitboard attacks_table[]) {
	struct BitboardSubsetIter subset_iter = {
		.original = magic->premask,
		.subset = 0,
	};
	do {
		size_t i = (subset_iter.subset * magic->multiplier) >> magic->rshift;
		Bitboard attacks = bb_rook(sq, subset_iter.subset);
		attacks_table[i] = attacks;
	} while (bb_subset_iter(&subset_iter));
}

void
bb_init_rook(void)
{
	size_t offset = 0;
	for (Square sq = 0; sq <= SQUARE_MAX; sq++) {
		init_rook_attack_table(sq, MAGICS + sq, BB_ATTACKS_ROOK + offset);
		BB_OFFSETS_ROOK[sq] = offset;
		BB_MASK_ROOK[sq] = MAGICS[sq].premask;
		BB_SHIFTS_ROOK[sq] = MAGICS[sq].rshift;
		BB_MULTIPLIERS_ROOK[sq] = MAGICS[sq].multiplier;
		BB_POSTMASK_ROOK[sq] = MAGICS[sq].postmask;
		offset += MAGICS[sq].end - MAGICS[sq].start + 3;
	}
}

size_t
magics_size_in_kib(void)
{
	size_t total = 0;
	for (Square sq = 0; sq <= SQUARE_MAX; sq++) {
		total += (MAGICS[sq].end - MAGICS[sq].start) * sizeof(Bitboard);
	}
	return total / 1024;
}

void
bb_init(void)
{
	static bool done = false;
	if (done) {
		return;
	}
	for (Square sq = 0; sq <= SQUARE_MAX; sq++) {
		BB_ATTACKS_BY_KNIGHT[sq] = bb_attacks_by_offsets(sq, OFFSETS_KNIGHT);
		BB_ATTACKS_BY_KING[sq] = bb_attacks_by_offsets(sq, OFFSETS_KING);
	}
	bb_init_rook();
	bb_init_bishop();
	done = true;
}
