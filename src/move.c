#include <stdlib.h>
#include <stdbool.h>
#include "move.h"

struct Move
mv_init(const struct Coordinates from, const struct Coordinates to) {
	struct Move mv = {
		.from = from,
		.to = to,
	};
	mv_set_direction(mv);
	return mv;
}

bool
mv_is_idempotent(const struct Move mv) {
	return coord_eq(mv.from, mv.to);
}

enum Direction
mv_set_direction(struct Move mv) {
	uint8_t horizontal_movement = mv_rank_change(mv);
	uint8_t vertical_movement = mv_file_change(mv);
	if (horizontal_movement == 0) {
		return DIR_VERTICAL;
	} else if (vertical_movement == 0) {
		return DIR_HORIZONTAL;
	} else if (horizontal_movement == vertical_movement) {
		return DIR_DIAGONAL;
	// 'x^y == 3' if and only if 'x==1 && y==2' or 'x==2 && y==1',
	// assuming that both 'x' and 'y' are less than 8 and posivite.
	} else if ((horizontal_movement ^ vertical_movement) == 3) {
		return DIR_L_SHAPED;
	} else {
		return DIR_NONE;
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
