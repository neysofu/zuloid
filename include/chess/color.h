/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_CHESS_COLOR_H
#define Z64C_CHESS_COLOR_H

#include "chess/coordinates.h"

enum Color
{
	COLOR_WHITE = 0,
	COLOR_BLACK = 1,
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
