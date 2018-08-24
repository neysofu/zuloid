#pragma once

#include <stdint.h>
#include "coord.h"

// As an extension, any non-zero integer represents COLOR_BLACK, while 0 is for
// COLOR_WHITE.
enum Color {
	COLOR_WHITE = 0,
	COLOR_BLACK = 1,
	// WARNING: can only be used as a marker for game results, not as argument
	// to any of the following functions or anywhere near 'struct Board'.
	COLOR_NONE = 2,
};

// Returns the rank in which pawns of 'color' promote.
Rank
color_promoting_rank(enum Color color);

// Returns the rank in which non-pawn pieces of 'color' are placed at the start
// of the game.
Rank
color_home_rank(enum Color color);

// Returns the rank in which pawns of 'color' are placed at the start of the
// game.
Rank
color_pawn_rank(enum Color color);

// Returns the rank in which pawns of 'color' end up after making an en-passant
// capture.
Rank
color_en_passant_rank(enum Color color);

// Returns the opposite color to 'color'.
enum Color
color_other(enum Color color);
