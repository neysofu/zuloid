#ifndef ZORRO_CHESS_FIND_MAGICS_H
#define ZORRO_CHESS_FIND_MAGICS_H

#include "chess/coordinates.h"
#include <stdlib.h>
#include <stdint.h>

struct Magic {
	Bitboard premask;
	uint64_t multiplier;
	short rshift;
	Bitboard postmask;
	size_t start;
	size_t end;
};

void
magic_find(struct Magic *magic, Square square, Bitboard *attacks_table);

#endif
