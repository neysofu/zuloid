/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

/* Allows better optimizations for `position_hash`. */
#define XXH_INLINE_ALL

#include "chess/position.h"
#include "chess/castling.h"
#include "chess/color.h"
#include "chess/coordinates.h"
#include "chess/move.h"
#include "chess/piece_types.h"
#include "utils.h"
#include "xxHash/xxhash.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
position_set_piece_at_square(struct Position *position, Square square, struct Piece piece)
{
	Bitboard bitboard = square_to_bitboard(square);
	enum PieceType ptype = piece.piece_type;
	switch (ptype) {
		case PIECE_TYPE_NONE:
			return;
		case PIECE_TYPE_QUEEN:
			position->occupancy_by_piece_type[PIECE_TYPE_BISHOP] |= bitboard;
			position->occupancy_by_piece_type[PIECE_TYPE_ROOK] |= bitboard;
			break;
		default:
			position->occupancy_by_piece_type[piece.piece_type] |= bitboard;
	}
	position->occupancy_by_color[piece.color] |= bitboard;
}

void
position_list_pieces_by_square(const struct Position *position, struct Piece *ptr)
{
	for (Square i = 0; i <= SQUARE_MAX; i++) {
		Bitboard bb = square_to_bitboard(i);
		ptr[i].color = position->occupancy_by_color[COLOR_BLACK] && bb;
	}
}

void
position_flip_side_to_move(struct Position *position)
{
	position->side_to_move ^= 1;
}

const struct Position POSITION_DEFAULT = {
	.occupancy_by_color =
	  {
	    0x0303030303030303,
	    0xc0c0c0c0c0c0c0c0,
	  },
	.occupancy_by_piece_type =
	  {
	    0x4242424242424242,
	    0x0081000000008100,
	    0x0000810081810000,
	    0x0000008100000000,
	    0x8100000081000081,
	  },
	.side_to_move = COLOR_WHITE,
	.castling_rights = CASTLING_RIGHTS_ALL,
	.reversible_moves_count = 0,
	.moves_count = 1,
};

const struct Position POSITION_EMPTY = {
	.moves_count = 1,
};
