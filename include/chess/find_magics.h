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

const struct Magic MAGICS[SQUARES_COUNT];
const struct Magic MAGICS_BISHOP[SQUARES_COUNT];

void
magic_find_rook(struct Magic *magic, Square square);

void
magic_find_bishop(struct Magic *magic, Square square);

#endif
