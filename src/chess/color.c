/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chess/color.h"
#include <assert.h>

char
color_to_char(Color color)
{
	assert(color != COLOR_NONE);
	return color ? 'b' : 'w';
}

Rank
color_promoting_rank(Color color) {
	assert(color != COLOR_NONE);
	return color ? 0 : 7;
}

Rank
color_home_rank(Color color) {
	assert(color != COLOR_NONE);
	return color ? 7 : 0;
}

Rank
color_pawn_rank(Color color) {
	assert(color != COLOR_NONE);
	return color ? 6 : 1;
}

Rank
color_en_passant_target_rank(Color color) {
	assert(color != COLOR_NONE);
	return color ? 2 : 5;
}

Color
color_other(Color color)
{
	assert(color != COLOR_NONE);
	return !color;
}
