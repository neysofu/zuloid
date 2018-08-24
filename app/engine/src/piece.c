#include <string.h>
#include "piece.h"

char
piece_to_char(enum Piece piece) {
	return "pnbrkq-"[piece];
}

enum Piece
char_to_piece(char c) {
	switch (c) {
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
