/* SPDX-License-Identifier: GPL-3.0-only */

#ifndef ZULOID_CHESS_MAGIC_H
#define ZULOID_CHESS_MAGIC_H

#include "chess/coordinates.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct Magic
{
	Bitboard premask;
	uint64_t multiplier;
	short rshift;
	Bitboard postmask;
};

void
magic_find_rook(struct Magic *magic, Square square);

void
magic_find_bishop(struct Magic *magic, Square square);

int
magics_export(const struct Magic *magics, const char *identifier, FILE *stream);

#endif
