#include "coordinates.h"

#pragma once
enum Color {
	// The promoting rank of both players is encoded in their color.
	COLOR_WHITE = 7,
	COLOR_BLACK = 0,
};

struct ColorSpecificInfo {
	struct Coordinates king;
};

Rank
color_promoting_rank(const enum Color color);
