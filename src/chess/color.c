/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chess/color.h"

char
color_to_char(enum Color color)
{
	return color ? 'b' : 'w';
}

Rank
color_promoting_rank(enum Color color) {
	return color ? 0 : 7;
}

Rank
color_home_rank(enum Color color) {
	return color ? 7 : 0;
}

Rank
color_pawn_rank(enum Color color) {
	return color ? 6 : 1;
}

Rank
color_en_passant_target_rank(enum Color color) {
	return color ? 2 : 5;
}

enum Color
color_other(enum Color color)
{
	return !color;
}
