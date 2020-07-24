#ifndef ZORRO_CHESS_COLOR_H
#define ZORRO_CHESS_COLOR_H

#include "chess/coordinates.h"

enum Color
{
	COLOR_WHITE = 0,
	COLOR_BLACK = 1,
};

enum
{
	COLORS_COUNT = 2,
};

char
color_to_char(enum Color color);

Rank
color_promoting_rank(enum Color color);

Rank
color_home_rank(enum Color color);

Rank
color_pawn_rank(enum Color color);

Rank
color_en_passant_target_rank(enum Color color);

Rank
color_double_push_rank(enum Color color);

enum Color
color_other(enum Color color);

#endif
