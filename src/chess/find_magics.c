#include "chess/find_magics.h"
#include "chess/bb.h"
#include "mt-64/mt-64.h"
#include "utils.h"
#include <assert.h>
#include <libpopcnt/libpopcnt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Magic MAGICS[SQUARES_COUNT] = { 0 };

Bitboard
slider_attacks(Square sq, Bitboard occupancy, const short delta[4][2])
{
	Bitboard bb = 0;
	for (int i = 0; i < 4; i++) {
		int df = delta[i][0];
		int dr = delta[i][1];
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

Bitboard
bb_rook(Square sq, Bitboard occupancy)
{
	const short rook_offsets[4][2] = { { -1, 0 }, { 0, -1 }, { 0, 1 }, { 1, 0 } };
	return slider_attacks(sq, occupancy, rook_offsets);
}

Bitboard
bb_sparse_random(void)
{
	return genrand64_int64() & genrand64_int64() & genrand64_int64();
}

Bitboard
bb_premask_rook(Square sq)
{
	Bitboard x = rank_to_bb(square_rank(sq)) & ~file_to_bb(0) & ~file_to_bb(7);
	Bitboard y = file_to_bb(square_file(sq)) & ~rank_to_bb(0) & ~rank_to_bb(7);
	return (x ^ y) & ~square_to_bb(sq);
}

struct BitboardSubsetIter
{
	Bitboard original;
	Bitboard subset;
};

Bitboard *
bb_subset_iter(struct BitboardSubsetIter *iter)
{
	// https://www.chessprogramming.org/Traversing_Subsets_of_a_Set
	iter->subset = (iter->subset - iter->original) & iter->original;
	if (iter->subset == 0) {
		return NULL;
	} else {
		return &(iter->subset);
	}
}

short
square_rshitf(Square square)
{
	File f = square_file(square);
	Rank r = square_rank(square);
	bool is_file_border = f == 0 || f == FILE_MAX;
	bool is_rank_border = r == 0 || r == RANK_MAX;
	if (is_file_border && is_rank_border) {
		return 52;
	} else if (is_file_border || is_rank_border) {
		return 53;
	} else {
		return 54;
	}
}

void
magic_find(struct Magic *magic, Square square, Bitboard *attacks_table)
{
	size_t attacks_table_size = sizeof(Bitboard) * 4096;
	magic->premask = bb_premask_rook(square);
	magic->rshift = square_rshitf(square);
	magic->start = 4095;
	magic->end = 0;
	while (true) {
		memset(attacks_table, 0, attacks_table_size);
		magic->multiplier = bb_sparse_random();
		struct BitboardSubsetIter subset_iter = {
			.original = magic->premask,
			.subset = 0,
		};
		bool found_collisions = false;
		Bitboard *subset = NULL;
		while ((subset = bb_subset_iter(&subset_iter))) {
			size_t i = (*subset * magic->multiplier) >> magic->rshift;
			Bitboard *val = attacks_table + i;
			Bitboard attacks = bb_rook(square, *subset);
			if (*val && *val != attacks) {
				found_collisions = true;
				break;
			} else {
				*val = attacks;
			}
		}
		if (!found_collisions) {
			break;
		}
	}
	for (size_t i = 0; i < 4096; i++) {
		if (attacks_table[i]) {
			magic->start = i;
			break;
		}
	}
	for (size_t i = 4095; i > 0; i--) {
		if (attacks_table[i]) {
			magic->end = i;
			break;
		}
	}
}
