/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_CHESS_COLOR_H
#define Z64C_CHESS_COLOR_H

#include "chess/coordinates.h"

typedef int_fast8_t Color;

enum
{
	COLOR_WHITE = 0,
	COLOR_BLACK = 1,
	COLOR_NONE = -1,
};

char
color_to_char(Color c);

Rank
color_promoting_rank(Color c);

Rank
color_home_rank(Color c);

Rank
color_pawn_rank(Color c);

Rank
color_en_passant_target_rank(Color c);

Color
color_other(Color c);

#endif
