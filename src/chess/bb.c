/* SPDX-License-Identifier: GPL-3.0-only */

#include "chess/bb.h"
#include "debug.h"
#include "mt-64/mt-64.h"
#include "utils.h"
#include <assert.h>
#include <libpopcnt/libpopcnt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

Bitboard
bb_next_subset(Bitboard mask, Bitboard previous_subset)
{
	// https://www.chessprogramming.org/Traversing_Subsets_of_a_Set
	// https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetKernighan
	return (previous_subset - mask) & mask;
}

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
bb_rays_rook(Square sq)
{
	Bitboard x = rank_to_bb(square_rank(sq)) & ~file_to_bb(0) & ~file_to_bb(7);
	Bitboard y = file_to_bb(square_file(sq)) & ~rank_to_bb(0) & ~rank_to_bb(7);
	return x ^ y;
}
