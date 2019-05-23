/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_CHESS_FEN_H
#define Z64C_CHESS_FEN_H

#include "chess/position.h"

/*  64 + | pieces
 *   7 + | slashes
 *   1 + | space
 *   1 + | active color
 *   1 + | space
 *   4 + | castling rights
 *   1 + | space
 *   2 + | en-passant target square
 *   1 + | space
 *   6 + | half-moves clock
 *   1 + | space
 *   6 + | full-moves
 * -----
 *  95 plus some extra safety space. */

enum
{
	FEN_SIZE = 128
};

char *
fen_new_from_position(const struct Position *position);

int
position_init_from_fen(struct Position *position, char *fen);

#endif
