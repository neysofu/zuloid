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
str_to_coord(const char *str) {
	if (strlen(str) < 2) {
		return COORD_NONE;
	}
	return coord_new(str[0] - 'a', str[1] - '1');
}

char
rank_to_char(Rank rank) {
	return rank + 'a';
}

bool
rank_is_valid(Rank rank) {
	return rank < 8;
}

char
file_to_char(File file) {
	return file + '1';
}

bool
file_is_valid(File file) {
	return file < 8;
}

bool
coord_eq(const struct Coord coord_1, const struct Coord coord_2) {
	return coord_1.rank == coord_2.rank && coord_1.file == coord_2.file;
}

bool
coord_is_valid(const struct Coord coord) {
	return coord.rank < BOARD_SIDE_LENGTH && coord.file < BOARD_SIDE_LENGTH;
}

enum Dir
coord_dir(const struct Coord coord_1, const struct Coord coord_2) {
	uint8_t horizontal_movement = abs(coord_1.file - coord_2.file);
	uint8_t vertical_movement = abs(coord_1.rank - coord_2.rank);
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

const Rank RANK_NONE = 42;
const File FILE_NONE = 9;
