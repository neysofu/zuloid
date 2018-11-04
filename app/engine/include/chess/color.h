/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @file color.h
 * @brief Chess information by color.
 */

#pragma once

#include "coord.h"
#include <stdint.h>

// As an extension, any non-zero integer represents COLOR_BLACK, while 0
// represents COLOR_WHITE. COLOR_NONE is used in game results (draws), not
// as argument to any of the following functions or anywhere near 'struct
// Board'.
enum Color
{
	COLOR_WHITE = 0,
	COLOR_BLACK = 1,
	COLOR_NONE = 2,
};

/// @brief Returns the rank in which pawns promote.
/// @pre @p color is not @c COLOR_NONE.
Rank
color_promoting_rank(enum Color color);

/// @brief Returns the rank in which non-pawn pieces are placed at the start
/// of the game.
/// @pre @p color is not @c COLOR_NONE.
Rank
color_home_rank(enum Color color);

/// @brief Returns the rank in which pawns are placed at the start of the
/// game.
/// @pre @p color is not @c COLOR_NONE.
Rank
color_pawn_rank(enum Color color);

// Returns the rank in which pawns end up after making an en-passant
// capture.
/// @pre @p color is not @c COLOR_NONE.
Rank
color_en_passant_rank(enum Color color);

/// @brief Returns the opposite color.
/// @pre @p color is not @c COLOR_NONE.
enum Color
color_other(enum Color color);
