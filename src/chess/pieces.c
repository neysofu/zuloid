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
