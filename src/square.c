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
	return (struct Square) {
		0, 0
	};
}
