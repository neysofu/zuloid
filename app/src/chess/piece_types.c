/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chess/piece_types.h"
#include "chess/color.h"
#include "chess/coordinates.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>

enum PieceType
piece_type(Piece p)
{
	return p & 0b111;
}

Color
piece_color(Piece p)
{
	return p >> 3;
}

char
piece_to_char(Piece p)
{
	assert(p <= PIECE_MAX);
	return "PNBKR\0Q\0pnbkr\0q\0"[p];
}

Piece
char_to_piece(char c)
{
	switch (c) {
		case 'P':
			return PIECE_TYPE_PAWN;
		case 'N':
			return PIECE_TYPE_KNIGHT;
		case 'B':
			return PIECE_TYPE_BISHOP;
		case 'K':
			return PIECE_TYPE_KING;
		case 'R':
			return PIECE_TYPE_ROOK;
		case 'Q':
			return PIECE_TYPE_QUEEN;
		case 'p':
			return PIECE_COLOR_BLACK | PIECE_TYPE_PAWN;
		case 'n':
			return PIECE_COLOR_BLACK | PIECE_TYPE_KNIGHT;
		case 'b':
			return PIECE_COLOR_BLACK | PIECE_TYPE_BISHOP;
		case 'k':
			return PIECE_COLOR_BLACK | PIECE_TYPE_KING;
		case 'r':
			return PIECE_COLOR_BLACK | PIECE_TYPE_ROOK;
		case 'q':
			return PIECE_COLOR_BLACK | PIECE_TYPE_QUEEN;
		default:
			return PIECE_TYPE_NONE;
	}
}

const Piece PIECE_MAX = PIECE_TYPE_QUEEN | PIECE_COLOR_BLACK;

// Bitboard
// rook_threats(Square sq)
//{
//	return FILE_TO_BITBOARD(SQUARE_RANK(sq)) ^ RANK_TO_BITBOARD(SQUARE_RANK(sq));
//}
//
// Bitboard
// knight_threats(Square sq)
//{
//	return BITBOARDS_KNIGHT_THREATS[sq];
//}
//
// Bitboard
// bishop_threats(Square sq)
//{
//	File file = SQUARE_FILE(sq);
//	Rank rank = SQUARE_RANK(sq);
//	return BITBOARDS_DIAGONALS[7 + rank - file] ^ BITBOARDS_ANTI_DIAGONALS[rank + file];
//}
//
// Bitboard
// king_threats(Square sq)
//{
//	return BITBOARDS_KING_THREATS[sq];
//}
//
// Bitboard
// queen_threats(Square sq)
//{
//	return bb_rook_threats(sq) | bb_bishop_threats(sq);
//}
