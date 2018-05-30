#include <stdbool.h>
#include "vector.h"
#include "chessboard.h"
#include "move.h"

bool
cb_discovers_attack(const struct Chessboard *cb, const struct Move mv, const struct Coordinates coord) {
	return false; // TODO
}

bool
cb_is_checkmate(const struct Chessboard *cb, const struct Move mv) {
	return true; // TODO
}
