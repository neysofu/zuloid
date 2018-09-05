#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "board.h"
#include "coord.h"
#include "dir.h"

char
file_to_char(File file) {
	assert(file_is_valid(file));
	return file + 'a';
}

File
char_to_file(char c) {
	assert(isalpha(c));
	return c - 'a';
}

bool
file_is_valid(File file) {
	return file < BOARD_SIDE_LENGTH;
}

char
rank_to_char(Rank rank) {
	assert(rank_is_valid(rank));
	return rank + '1';
}

Rank
char_to_rank(char c) {
	assert(isdigit(c));
	return c - '1';
}

bool
rank_is_valid(Rank rank) {
	return rank < BOARD_SIDE_LENGTH;
}

Coord
coord_new(File file, Rank rank) {
	assert(file_is_valid(file));
	assert(rank_is_valid(rank));
	return (rank << 3) | file;
}

uint64_t
coord_to_bb(Coord coord) {
	assert(coord_is_valid(coord));
	return 1LLU << coord;
}

void
coord_to_str(Coord coord, char *str) {
	assert(coord_is_valid(coord));
	str[0] = file_to_char(coord_file(coord));
	str[1] = rank_to_char(coord_rank(coord));
}

Coord
str_to_coord(char *str) {
	return coord_new(char_to_file(str[0]), char_to_rank(str[1]));
}

File
coord_file(Coord coord) {
	assert(coord_is_valid(coord));
	return coord & 0x7;
}

Rank
coord_rank(Coord coord) {
	assert(coord_is_valid(coord));
	return coord >> 3;
}

bool
coord_is_valid(Coord coord) {
	return coord < BOARD_NUM_SQUARES;
}

enum Dir
coord_dir(Coord coord_1, Coord coord_2) {
	assert(coord_is_valid(coord_1));
	assert(coord_is_valid(coord_2));
	uint8_t horizontal_shift = abs(coord_file(coord_1) - coord_file(coord_2));
	uint8_t vertical_shift = abs(coord_rank(coord_1) - coord_rank(coord_2));
	// TODO: bitwise hackery to speed this up with branchless execution.
	if (horizontal_shift == 0) {
		return DIR_VERTICAL;
	} else if (vertical_shift == 0) {
		return DIR_HORIZONTAL;
	} else if (horizontal_shift == vertical_shift) {
		return DIR_DIAGONAL;
	} else if (horizontal_shift + vertical_shift == 3) {
		return DIR_L_SHAPED;
	} else {
		return DIR_NONE;
	}
}

const File FILE_NONE = (1 << 8) - 1;
const Rank RANK_NONE = (1 << 8) - 1;
const Coord COORD_NONE = (1 << 8) - 1;
