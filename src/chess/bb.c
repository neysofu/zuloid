#include "chess/bb.h"
#include "chess/find_magics.h"
#include "mt-64/mt-64.h"
#include "utils.h"
#include <assert.h>
#include <libpopcnt/libpopcnt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

Bitboard BB_ATTACKS_BY_KNIGHT[SQUARES_COUNT] = { 0 };
Bitboard BB_ATTACKS_BY_KING[SQUARES_COUNT] = { 0 };

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
bb_print(Bitboard bb)
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
bb_attacks_bishop(Square sq, Bitboard obstacles)
{
	size_t i = (BB_MAGICS_BISHOP[sq] * BB_MULTIPLIERS_BISHOP[sq]) >> BB_SHIFTS_BISHOP[sq];
	return BB_ATTACKS_BISHOP[i];
}

Bitboard
bb_attacks_rook(Square sq, Bitboard obstacles)
{
	size_t i = (BB_MAGICS_ROOK[sq] * BB_MULTIPLIERS_ROOK[sq]) >> BB_SHIFTS_ROOK[sq];
	return BB_ATTACKS_ROOK[i];
}

Bitboard
bb_bishop(Square sq, Bitboard occupancy)
{
	const short bishop_offsets[4][2] = { { -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 1 } };
	return slider_attacks(sq, occupancy, bishop_offsets);
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
	return;
}

void
bb_init_rook(void)
{
	size_t offset = 0;
	for (Square sq = 0; sq <= SQUARES_COUNT; sq++) {
		bool magic_was_found = false;
		while (!magic_was_found) {
			Bitboard key = 0;
			BB_MAGICS_ROOK[sq] = bb_sparse_random();
			BB_MASK_ROOK[sq] = bb_rays_rook(sq);
			BB_POSTMASK_ROOK[sq] =
			  file_to_bb(square_file(sq)) | rank_to_bb(square_rank(sq));
			do {
				size_t index = ((key & BB_MASK_ROOK[sq]) * BB_MASK_ROOK[sq]) >> 14;
				Bitboard *val = BB_ATTACKS_ROOK + index + offset;
				if (index >= 16384) {
					break;
				} else if (*val && *val != bb_rook(sq, key)) {
					bb_print(*val);
					puts("[BUG] Invalid magics.");
					exit(1);
				} else if (!*val) {
					bb_print(*val);
					*val |= bb_rook(sq, key);
					bb_print(*val);
				}
				// https://www.chessprogramming.org/Traversing_Subsets_of_a_Set
			} while ((key = (key - BB_MASK_ROOK[sq]) & BB_MASK_ROOK[sq]));
			offset += 1ULL << (64 - BB_SHIFTS_ROOK[sq]);
		}
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
	for (Square sq = 0; sq <= SQUARE_MAX; sq++) {
		BB_ATTACKS_BY_KNIGHT[sq] = bb_attacks_by_offsets(sq, OFFSETS_KNIGHT);
		BB_ATTACKS_BY_KING[sq] = bb_attacks_by_offsets(sq, OFFSETS_KING);
	}
	size_t offset = 0;
	for (Square sq = 0; sq <= SQUARE_MAX; sq++) {
		magic_find(&(MAGICS[sq]), sq, BB_ATTACKS_ROOK + offset);
		offset += MAGICS[sq].end - MAGICS[sq].start;
	}
	// bb_init_rook();
	// bb_init_bishop();
}
