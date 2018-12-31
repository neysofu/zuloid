/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chess/color.h"
#include <assert.h>

char
color_to_char(Color c)
{
	assert(c != COLOR_NONE);
	return c ? 'b' : 'w';
}

Rank
color_promoting_rank(Color c);

Rank
color_home_rank(Color c);

Rank
color_pawn_rank(Color c);

Rank
color_en_passant_target_rank(Color c);

Color
color_other(Color c)
{
	assert(c != COLOR_NONE);
	return !c;
}
