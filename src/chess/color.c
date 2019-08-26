/* Copyright (c) Filippo Costa - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * PROPRIETARY AND CONFIDENTIAL */

#include "chess/color.h"

char
color_to_char(enum Color color)
{
	return color ? 'b' : 'w';
}

Rank
color_promoting_rank(enum Color color)
{
	return color ? 0 : 7;
}

Rank
color_home_rank(enum Color color)
{
	return color ? 7 : 0;
}

Rank
color_pawn_rank(enum Color color)
{
	return color ? 6 : 1;
}

Rank
color_en_passant_target_rank(enum Color color)
{
	return color ? 2 : 5;
}

Rank
color_double_push_rank(enum Color color)
{
	return color ? 4 : 3;
}

enum Color
color_other(enum Color color)
{
	return !color;
}
