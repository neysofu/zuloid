#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "coord.h"
#include "board.h"

struct Coord
coord_new(Rank rank, File file) {
	return (struct Coord) {
		.rank = rank,
		.file = file,
	};
}

char *
coord_to_str(const struct Coord coord, char *str) {
	if (!str) {
		char *str = malloc(2);
		if (!str) {
			return str;
		}
	}
	str[0] = rank_to_char(coord.rank);
	str[1] = file_to_char(coord.file);
	return str;
}

struct Coord
str_to_coord(const char *coord) {
	if (strlen(coord) < 2) {
		return COORD_NONE;
	}
	return coord_new(coord[0] - 'a', coord[1] - '1');
}

char
rank_to_char(Rank rank) {
	return rank + 'a';
}

char
file_to_char(File file) {
	return file + '1';
}

bool
coord_eq(const struct Coord coord1, const struct Coord coord2) {
	return coord1.rank == coord2.rank && coord1.file == coord2.file;
}

bool
coord_is_valid(const struct Coord coord) {
	return coord.rank < BOARD_SIDE_LENGTH && coord.file < BOARD_SIDE_LENGTH;
}

enum Dir
coord_dir(const struct Coord coord1, const struct Coord coord2) {
	uint8_t horizontal_movement = abs(coord1.file - coord2.file);
	uint8_t vertical_movement = abs(coord1.rank - coord2.rank);
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

const struct Coord COORD_NONE = {
	.rank = 42,
	.file = 9,
};
