#pragma once

// In the context of chess, pieces can move in a number of different
// directions. Some pieces (pawns, knigths, and bishops) are capable of moving
// only in one direction, while others have more freedom of movement. It is
// worth of notice that the direction between two squares is symmetrical, i.e.
// the direction beween A and B is the same between B and A.
// A 'DIR_NONE' direction is useful for encoding illegal moves.
enum Dir {
	DIR_NONE,
	DIR_HORIZONTAL,
	DIR_VERTICAL,
	DIR_DIAGONAL,
	DIR_L_SHAPED,
};
