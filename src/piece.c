bool
cb_is_move_pseudolegal_pawn(const struct Chessboard cb, const struct Move mv) {
	if (mv_rank_change(mv) != 1) {
		return false;
	}
	bool is_capture = cb_is_move_capture(cb, mv);
	switch (mv.direction) {
		case DIR_DIAGONAL:
			return is_capture;
		case DIR_VERTICAL:
			return !is_capture;
		default:
			return false;
	}
}

bool
cb_is_move_pseudolegal_knight(const struct Chessboard cb, const struct Move mv) {
	return mv.direction == DIR_L_SHAPED;
}

bool
cb_is_move_pseudolegal_bishop(const struct Chessboard cb, const struct Move mv) {
	return mv.direction == DIR_DIAGONAL && !mv_is_obstructed(mv);
}

bool
cb_is_move_pseudolegal_rook(const struct Chessboard cb, const struct Move mv) {
	return (mv.direction == DIR_HORIZONTAL ||
		    mv.direction == DIR_VERTICAL) &&
		   !mv_is_obstructed(mv);
}

bool
cb_is_move_pseudolegal_queen(const struct Chessboard cb, const struct Move mv) {
	return (mv.direction == DIR_HORIZONTAL ||
		    mv.direction == DIR_VERTICAL ||
		    mv.direction == DIR_DIAGONAL) &&
		   !mv_is_obstructed(mv);
}
