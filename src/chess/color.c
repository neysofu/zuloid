#include "chess/color.h"
#include "chess/mnemonics.h"

char
color_to_char(enum Color color)
{
	return color ? 'b' : 'w';
}

Rank
color_promoting_rank(enum Color color)
{
	return color ? R_1 : R_8;
}

Rank
color_home_rank(enum Color color)
{
	return color ? R_8 : R_1;
}

Rank
color_pawn_rank(enum Color color)
{
	return color ? R_7 : R_2;
}

Rank
color_en_passant_target_rank(enum Color color)
{
	return color ? R_3 : R_6;
}

Rank
color_double_push_rank(enum Color color)
{
	return color ? R_4 : R_5;
}

enum Color
color_other(enum Color color)
{
	return !color;
}
