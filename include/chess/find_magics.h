#ifndef ZORRO_CHESS_FIND_MAGICS_H
#define ZORRO_CHESS_FIND_MAGICS_H

#include "chess/coordinates.h"
#include <stdint.h>
#include <stdlib.h>

struct Magic
{
	Bitboard premask;
	uint64_t multiplier;
	short rshift;
	Bitboard postmask;
	size_t start;
	size_t end;
};

struct Magic MAGICS[SQUARES_COUNT];
struct Magic MAGICS_BISHOP[SQUARES_COUNT];

void
magic_find(struct Magic *magic, Square square, Bitboard *attacks_table);

void
magic_find_bishop(struct Magic *magic, Square square, Bitboard *attacks_table);

struct BitboardSubsetIter
{
	Bitboard original;
	Bitboard subset;
};

Bitboard *
bb_subset_iter(struct BitboardSubsetIter *iter);

#endif
