#pragma once

#include "piece.h"
#include "color.h"

struct Square {
	enum Piece piece;
	enum Color color;
};

char
square_to_char(struct Square square);

struct Square
char_to_square(char c);

extern const struct Square SQUARE_NONE;
