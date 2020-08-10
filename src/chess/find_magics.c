#include "chess/find_magics.h"
#include "chess/bb.h"
#include "chess/bb_subset_iter.h"
#include "chess/diagonals.h"
#include "debug.h"
#include "mt-64/mt-64.h"
#include "utils.h"
#include <assert.h>
#include <libpopcnt/libpopcnt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

Bitboard
bb_premask_bishop(Square sq)
{
	Bitboard diagonal1 = square_a1h8_diagonal(sq);
	Bitboard diagonal2 = square_a8h1_diagonal(sq);
	Bitboard borders = file_to_bb(0) | file_to_bb(7) | rank_to_bb(0) | rank_to_bb(7);
	return (diagonal1 ^ diagonal2) & ~borders;
}

size_t
find_start_of_attacks_table(Bitboard attacks_table[])
{
	for (size_t i = 0; i < 4096; i++) {
		if (attacks_table[i]) {
			return i;
		}
	}
	return 4095;
}

size_t
find_end_of_attacks_table(Bitboard attacks_table[])
{
	for (size_t i = 4095; i > 0; i--) {
		if (attacks_table[i]) {
			return i;
		}
	}
	return 0;
}

bool
verify_magic_candidate(const struct Magic *magic,
                       Square sq,
                       Bitboard buffer[],
                       Bitboard (*attacker)(Square, Bitboard))
{
	struct BitboardSubsetIter subset_iter = {
		.original = magic->premask,
		.subset = 0,
	};
	do {
		size_t i = (subset_iter.subset * magic->multiplier) >> magic->rshift;
		Bitboard *val = buffer + i;
		Bitboard attacks = attacker(sq, subset_iter.subset);
		if (*val) {
			return false;
		} else {
			*val = attacks;
		}
	} while (bb_subset_iter(&subset_iter));
	return true;
}

void
magic_find_rook(struct Magic *magic, Square square)
{
	size_t attacks_table_size = sizeof(Bitboard) * 4096;
	Bitboard *attacks_table = malloc(sizeof(Bitboard) * 4096);
	exit_if_null(attacks_table);
	magic->premask = bb_premask_rook(square);
	magic->rshift = 52;
	do {
		memset(attacks_table, 0, attacks_table_size);
		magic->multiplier = bb_sparse_random();
	} while (!verify_magic_candidate(magic, square, attacks_table, bb_rook));
	magic->start = find_start_of_attacks_table(attacks_table);
	magic->end = find_end_of_attacks_table(attacks_table);
	free(attacks_table);
}

void
magic_find_bishop(struct Magic *magic, Square square)
{
	size_t attacks_table_size = sizeof(Bitboard) * 4096;
	Bitboard *attacks_table = malloc(sizeof(Bitboard) * 4096);
	exit_if_null(attacks_table);
	magic->premask = bb_premask_bishop(square);
	magic->rshift = 64 - popcount64(magic->premask);
	do {
		memset(attacks_table, 0, attacks_table_size);
		magic->multiplier = bb_sparse_random();
	} while (!verify_magic_candidate(magic, square, attacks_table, bb_bishop));
	magic->start = 0;
	magic->end = find_end_of_attacks_table(attacks_table);
	free(attacks_table);
}
