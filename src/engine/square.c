#include "square.h"
#include "piece.h"
#include "color.h"

char
square_to_char(struct Square square) {
	if (square.color == COLOR_WHITE || square.piece == PIECE_NONE) {
		return square.piece;
	} else {
		return square.piece - 'A' + 'a';
	}
}

struct Square
char_to_square(char c) {
	struct Square square;
	if (c == '.') {
		square = SQUARE_NONE;
	} else {
		if (c > 'a') {
			c -= ('a' - 'A');
			square.color = COLOR_BLACK;
		} else {
			square.color = COLOR_WHITE;
		}
		square.piece = c;
	}
	return square;
}

const struct Square SQUARE_NONE = {
	.piece = PIECE_NONE,
	.color = COLOR_NONE,
};
