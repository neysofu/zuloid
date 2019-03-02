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
	PIECE_TYPE_PAWN,
	PIECE_TYPE_KNIGHT,
	PIECE_TYPE_BISHOP,
	PIECE_TYPE_KING,
	PIECE_TYPE_ROOK,
	PIECE_TYPE_NONE,
	PIECE_TYPE_QUEEN,
};

struct Piece
{
	enum PieceType piece_type : 3;
	enum Color color : 1;
};

enum
{
	CORE_PIECE_TYPES_COUNT = 5,
};

extern const struct Piece PIECE_MAX;
extern const struct Piece PIECE_NONE;

char
piece_to_char(struct Piece piece);

struct Piece
char_to_piece(char c);

enum PieceType
direction_between_two_squares(Square sq0, Square sq1);

#endif
