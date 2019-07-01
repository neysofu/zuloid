/* Copyright (c) Filippo Costa - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * PROPRIETARY AND CONFIDENTIAL */

#ifndef Z64C_CHESS_COLOR_H
#define Z64C_CHESS_COLOR_H

#include "chess/coordinates.h"

enum Color
{
	COLOR_WHITE = 0,
	COLOR_BLACK = 1,

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

enum Color
color_other(enum Color color);

#endif
