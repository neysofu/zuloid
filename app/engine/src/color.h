#pragma once

#include "coord.h"

enum Color {
	COLOR_WHITE = 0,
	COLOR_BLACK = 1,
	COLOR_NONE = 2,
};

Rank
color_promoting_rank(const enum Color color);

Rank
color_pawn_rank(const enum Color color);

Rank
color_en_passant_rank(const enum Color color);

enum Color
color_other(enum Color color);
