#include <assert.h>
#include <string.h>
#include "chess/piece.h"

char
piece_to_char(enum Piece piece) {
	assert(piece != PIECE_NONE);
	return "pnbrk.q"[piece];
}

enum Piece
char_to_piece(char c) {
	switch (tolower(c)) {
		case 'p':
			return PIECE_PAWN;
		case 'n':
			return PIECE_KNIGHT;
		case 'b':
			return PIECE_BISHOP;
		case 'r':
			return PIECE_ROOK;
		case 'k':
			return PIECE_KING;
		case 'q':
			return PIECE_QUEEN;
		default:
			return PIECE_NONE;
	}
}
