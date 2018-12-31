/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * See
 *   https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation */

#ifndef Z64C_CHESS_POSITION_H
#define Z64C_CHESS_POSITION_H

#include "chess/castling.h"
#include "chess/color.h"
#include "chess/piece_types.h"
#include "chess/coordinates.h"
#include <stdbool.h>
#include <stdlib.h>

struct Position
{
	Bitboard occupancy_by_color[2];
	Bitboard occupancy_by_piece_type[CORE_PIECE_TYPES_COUNT];
	Color side_to_move;
	Square en_passant_target;
	CastlingRights castling_rights;
	int_fast8_t reversible_moves_count;
};

uint_fast64_t
position_hash(const struct Position *pos);

Color
position_color_at(const struct Position *pos, Square sq);

void
position_flip_side_to_move(struct Position *pos);

size_t
position_generate_legal_moves(const struct Position *pos);

// Returns the lowercase letter that FEN uses to represent the piece at 'coord';
// if none, returns ".".
char
position_square_to_char(const struct Position *pos, Square sq);

extern const struct Position POSITION_DEFAULT;

#endif
