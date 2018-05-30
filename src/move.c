bool
mv_is_idempotent(const struct Move mv) {
	return coord_eq(mv.from, mv.to);
}

enum Direction
mv_set_direction(struct Move mv) {
	uint8_t horizontal_movement = mv_rank_change(mv);
	uint8_t vertical_movement = mv_file_change(mv);
	switch (horizontal_movement ^ vertical_movement) {
		case horizontal_movement:
			return HORIZONTAL;
		case vertical_movement:
			return VERTICAL;
		case 0:
			return DIAGONAL;
		// 'x^y == 3' if and only if 'x==1 && y==2' or 'x==2 && y==1',
		// assuming that both 'x' and 'y' are less than 8 and posivite.
		case 3:
			return L_SHAPED;
		default:
			return NONE;
	}
}

uint8_t
mv_rank_change(const struct Move mv) {
	return abs(mv.from.rank - mv.to.rank);
}

uint8_t
mv_file_change(const struct Move mv) {
	return abs(mv.from.file - mv.to.file);
}
