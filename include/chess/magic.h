#ifndef ZULOID_CHESS_MAGIC_H
#define ZULOID_CHESS_MAGIC_H

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

void
magic_init(void);

Bitboard
bb_bishop_magic(Square sq, Bitboard obstacles);
Bitboard
bb_rook_magic(Square sq, Bitboard obstacles);

#endif
