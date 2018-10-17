#include "chess/color.h"
#include "chess/coord.h"
#include <stdlib.h>

Rank
color_promoting_rank(enum Color color)
{
	return color_home_rank(color_other(color));
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
color_en_passant_rank(enum Color color)
{
	return color ? 5 : 2;
}

enum Color
color_other(enum Color color)
{
	return !color;
}
