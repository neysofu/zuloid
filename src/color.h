#pragma once

#include "coord.h"

enum Color {
	COLOR_WHITE = 0,
	COLOR_BLACK = 1,
};

Rank
color_promoting_rank(const enum Color color);

enum Color
color_other(enum Color color);
