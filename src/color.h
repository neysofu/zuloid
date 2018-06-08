#pragma once

#include "coord.h"

enum Color {
	COLOR_WHITE,
	COLOR_BLACK,
};

Rank
color_promoting_rank(const enum Color color);

enum Color
color_other(enum Color color);
