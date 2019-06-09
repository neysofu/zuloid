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

enum
{
	CASTLING_RIGHT_NONE = 0,
	CASTLING_RIGHT_KINGSIDE = 0x1,
	CASTLING_RIGHT_QUEENSIDE = 0x2,
	CASTLING_RIGHTS_ALL = 0xf,

	POSITION_BB_COUNT = COLORS_COUNT + PRIMITIVE_PIECE_TYPES_COUNT,
};

/* Parses a FEN field into castling rights. */
int
string_to_castling_rights(const char *str);

struct Position
{
	Bitboard bb[POSITION_BB_COUNT];
	enum Color side_to_move;
	Square en_passant_target;
	int castling_rights;
	size_t reversible_moves_count;
	size_t moves_count;
};

/* Randomly sets up the chess position from Chess 960. */
void
position_init_960(struct Position *position);

/* Removes a piece from the board and replaces it with another one. */
void
position_set_piece_at_square(struct Position *position, Square square, struct Piece piece);

enum Color
position_toggle_side_to_move(struct Position *pos);

struct Piece
position_piece_at_square(const struct Position *position, Square square);

void
position_empty(struct Position *position);

extern const struct Position POSITION_INIT;

#endif
