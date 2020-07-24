#include "chess/pieces.h"
#include "chess/color.h"
#include "chess/coordinates.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>

char
piece_to_char(struct Piece piece)
{
	return "....PpNnBbRrKk....Qq"[piece.type * 2 + piece.color];
}

struct Piece
char_to_piece(char c)
{
	struct Piece piece;
	switch (tolower(c)) {
		case 'p':
			piece.type = PIECE_TYPE_PAWN;
			break;
		case 'n':
			piece.type = PIECE_TYPE_KNIGHT;
			break;
		case 'b':
			piece.type = PIECE_TYPE_BISHOP;
			break;
		case 'r':
			piece.type = PIECE_TYPE_ROOK;
			break;
		case 'k':
			piece.type = PIECE_TYPE_KING;
			break;
		case 'q':
			piece.type = PIECE_TYPE_QUEEN;
			break;
		default:
			piece.type = PIECE_TYPE_NONE;
			break;
	}
	piece.color = isupper(c) ? COLOR_WHITE : COLOR_BLACK;
	return piece;
}

const struct Piece PIECE_NONE = { .type = PIECE_TYPE_NONE, .color = COLOR_WHITE };

// Bitboard
// rook_threats(Square sq)
//{
//	return FILE_TO_BITBOARD(SQUARE_RANK(sq)) ^
// RANK_TO_BITBOARD(SQUARE_RANK(sq));
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
//	return BITBOARDS_DIAGONALS[7 + rank - file] ^
// BITBOARDS_ANTI_DIAGONALS[rank + file];
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
