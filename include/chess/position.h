/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * See
 *   https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation */

#ifndef Z64C_CHESS_POSITION_H
#define Z64C_CHESS_POSITION_H

#include "chess/color.h"
#include "chess/coordinates.h"
#include "chess/pieces.h"
#include <stdbool.h>
#include <stdlib.h>

enum CastlingRights
{
	CASTLING_RIGHT_W_OO = 1,
	CASTLING_RIGHT_B_OO = 2,
	CASTLING_RIGHT_W_OOO = 4,
	CASTLING_RIGHT_B_OOO = 8,
	CASTLING_RIGHTS_ALL = CASTLING_RIGHT_W_OO | CASTLING_RIGHT_B_OO | CASTLING_RIGHT_W_OOO |
	                      CASTLING_RIGHT_B_OOO,
};

struct Position
{
	Bitboard bb[COLORS_COUNT + PRIMITIVE_PIECE_TYPES_COUNT];
	enum Color side_to_move;
	bool is_en_passant_available;
	Square en_passant_target;
	enum CastlingRights castling_rights;
	size_t reversible_moves_count;
	size_t moves_count;
};

void
position_setup_960(struct Position *position);

void
position_set_piece_at_square(struct Position *position, Square square, struct Piece piece);

struct Piece
position_piece_at_square(const struct Position *position, Square square);

extern const struct Position POSITION_DEFAULT;
extern const struct Position POSITION_EMPTY;

#endif
