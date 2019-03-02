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
#include "chess/coordinates.h"
#include "chess/piece_types.h"
#include <stdbool.h>
#include <stdlib.h>

struct Position
{
	Bitboard occupancy_by_color[2];
	Bitboard occupancy_by_piece_type[CORE_PIECE_TYPES_COUNT];
	enum Color side_to_move;
	bool is_en_passant_available;
	Square en_passant_target;
	CastlingRights castling_rights;
	size_t reversible_moves_count;
	size_t moves_count;
};

void
position_set_piece_at_square(struct Position *position, Square square, struct Piece piece);

void
position_list_pieces_by_square(const struct Position *position, struct Piece ptr[]);

enum Color
position_color_at(const struct Position *position, Square square);

// Returns the lowercase letter that FEN uses to represent the piece at 'coord';
// if none, returns ".".
char
position_square_to_char(const struct Position *position, Square square);

extern const struct Position POSITION_DEFAULT;
extern const struct Position POSITION_EMPTY;

#endif
