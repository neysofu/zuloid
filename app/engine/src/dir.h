#pragma once

// The direction between two squares on a chessboard.
enum Dir {
	// Two squares on the same rank.
	DIR_HORIZONTAL = 0,
	// Two squares on the same file.
	DIR_VERTICAL = 1,
	// Two squares on the same diagonal.
	DIR_DIAGONAL = 2,
	// Two squares a knight's move away from each other.
	DIR_L_SHAPED = 3,
	// Two squares two queen's moves away from each other. (This is also the
	// direction between a square to itself.)
	DIR_NONE = 4,
};
