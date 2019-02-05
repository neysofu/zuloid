/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_CHESS_PIECE_H
#define Z64C_CHESS_PIECE_H

#include "chess/color.h"
#include "chess/coordinates.h"
#include <stdbool.h>

typedef int_fast8_t Piece;

enum PieceType
{
	PIECE_TYPE_PAWN = 0x0,
	PIECE_TYPE_KNIGHT = 0x1,
	PIECE_TYPE_BISHOP = 0x2,
	PIECE_TYPE_KING = 0x3,
	PIECE_TYPE_ROOK = 0x4,
	PIECE_TYPE_NONE = 0x5,
	PIECE_TYPE_QUEEN = 0x6,
};

enum PieceColor
{
	PIECE_COLOR_WHITE = 0,
	PIECE_COLOR_BLACK = 0x8,
};

enum
{
	CORE_PIECE_TYPES_COUNT = 5,
};

extern const Piece PIECE_MAX;
extern const Piece PIECE_EMPTY;

bool
piece_is_valid(Piece p);

Color
piece_color(Piece p);

enum PieceType
piece_type(Piece piece);

Piece
piece_colorless(Piece p);

char
piece_to_char(Piece p);

Piece
char_to_piece(char c);

Bitboard
pawn_threats(Square sq);
Bitboard
knight_threats(Square sq);
Bitboard
bishop_threats(Square sq);
Bitboard
rook_threats(Square sq);
Bitboard
king_threats(Square sq);

Piece
direction_between_two_squares(Square sq0, Square sq1);

#endif
