/* SPDX-License-Identifier: GPL-3.0-only */

#include "chess/magic.h"
#include "chess/bb.h"
#include "chess/diagonals.h"
#include "chess/generated/magics_bishop.h"
#include "chess/generated/magics_rook.h"
#include "chess/mnemonics.h"
#include "chess/threats.h"
#include "libpopcnt/libpopcnt.h"
#include "mt-64/mt-64.h"
#include "utils.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
magics_export(const struct Magic *magics, const char *identifier, FILE *stream)
{
	fprintf(stream, "\nconst struct Magic %s[SQUARES_COUNT] = {\n", identifier);
	for (Square sq = 0; sq < SQUARES_COUNT; sq++) {
		struct Magic magic = magics[sq];
		fprintf(stream,
		        "\t[0%o] = {"
		        " .premask = 0x%" PRIx64 "ULL,"
		        " .multiplier = 0x%" PRIx64 "ULL,"
		        " .rshift = %d,"
		        " .postmask = 0x%" PRIx64 "ULL },\n",
		        sq,
		        magic.premask,
		        magic.multiplier,
		        magic.rshift,
		        magic.postmask);
	}
	fprintf(stream, "};\n");
	return 0;
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

Bitboard
bb_premask_bishop(Square sq)
{
	Bitboard diagonal1 = square_a1h8_diagonal(sq);
	Bitboard diagonal2 = square_a8h1_diagonal(sq);
	Bitboard borders = file_to_bb(0) | file_to_bb(7) | rank_to_bb(0) | rank_to_bb(7);
	return (diagonal1 ^ diagonal2) & ~borders;
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
	Bitboard subset = 0;
	do {
		size_t i = (subset * magic->multiplier) >> magic->rshift;
		Bitboard *val = buffer + i;
		Bitboard attacks = attacker(sq, subset);
		if (*val) {
			return false;
		} else {
			*val = attacks;
		}
	} while ((subset = bb_next_subset(magic->premask, subset)));
	return true;
}

void
find_magic(struct Magic *magic,
           Square square,
           Bitboard (*attacker)(Square, Bitboard),
           Bitboard (*premasker)(Square))
{
	size_t attacks_table_size = sizeof(Bitboard) * 4096;
	Bitboard *attacks_table = malloc(sizeof(Bitboard) * 4096);
	exit_if_null(attacks_table);
	magic->premask = premasker(square);
	magic->postmask = UINT64_MAX;
	magic->rshift = 64 - popcnt64(magic->premask);
	do {
		memset(attacks_table, 0, attacks_table_size);
		magic->multiplier = bb_sparse_random();
	} while (!verify_magic_candidate(magic, square, attacks_table, attacker));
	free(attacks_table);
}

void
magic_find_rook(struct Magic *magic, Square square)
{
	find_magic(magic, square, threats_by_rook_no_init, bb_premask_rook);
}

void
magic_find_bishop(struct Magic *magic, Square square)
{
	find_magic(magic, square, threats_by_bishop_no_init, bb_premask_bishop);
}
