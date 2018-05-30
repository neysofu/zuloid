#include <stdbool.h>
#include "chessboard.h"
#include "move.h"

bool
cb_is_check(const struct Chessboard cb, const struct Move mv) {
	struct Move king_capture = mv_init(mv.to, cb.white_king);
	if (cb_is_move_legal(cb, king_capture)) {
		return true;
	} else {
		return cb_discovers_attack(cb, mv, cb.white_king);
	}
}

bool
cb_discovers_attack(const struct Chessboard cb, const struct Move mv, const struct Coordinates coord) {
	switch (mv_init(mv.from, coord).direction) {
		case DIR_HORIZONTAL:

		case DIR_VERTICAL:

		case DIR_DIAGONAL:

		default:
			return false;
	}
}

bool
cb_is_checkmate(const struct Chessboard cb, const struct Move mv) {

}
bool
cb_discovers_attack(const struct Chessboard cb, const struct Move mv) {
	return false;
	// TODO
}
