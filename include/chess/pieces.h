/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_CHESS_PIECE_H
#define Z64C_CHESS_PIECE_H

#include "chess/color.h"
#include "chess/coordinates.h"
#include <stdbool.h>
#include <stdlib.h>

enum PieceType
{
	PIECE_TYPE_NONE = 0,
	PIECE_TYPE_PAWN = 2,
	PIECE_TYPE_KNIGHT = 3,
	PIECE_TYPE_BISHOP = 4,
	PIECE_TYPE_ROOK = 5,
	PIECE_TYPE_KING = 6,
	PIECE_TYPE_QUEEN = PIECE_TYPE_ROOK + PIECE_TYPE_BISHOP,
	PIECE_TYPE_FIRST_PRIMITIVE = PIECE_TYPE_PAWN,
	PIECE_TYPE_LAST_PRIMITIVE = PIECE_TYPE_KING,
	PRIMITIVE_PIECE_TYPES_COUNT =
	  PIECE_TYPE_LAST_PRIMITIVE - PIECE_TYPE_FIRST_PRIMITIVE + 1,
};

struct Piece
{
	enum PieceType type : 4;
	enum Color color : 2;
};

extern const struct Piece PIECE_NONE;

char
piece_to_char(struct Piece piece);

struct Piece
char_to_piece(char c);

enum PieceType
direction_between_two_squares(Square sq0, Square sq1);

#endif
