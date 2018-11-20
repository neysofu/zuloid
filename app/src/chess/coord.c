/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "chess/coord.h"
#include "chess/board.h"
#include "chess/dir.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// FILE
// ====

bool
file_is_in_bounds(File file)
{
	return file < BOARD_SIDE_LENGTH;
}

char
file_to_char(File file)
{
	assert(file_is_in_bounds(file));
	return file + 'a';
}

File
char_to_file(char c)
{
	if (isalpha(c)) {
		File file = tolower(c) - 'a';
		assert(file_is_in_bounds(file));
		return file;
	} else {
		return FILE_NONE;
	}
}

const File FILE_NONE = (1 << 8) - 1;

// RANK
// ====

bool
rank_is_in_bounds(Rank rank)
{
	return rank < BOARD_SIDE_LENGTH;
}

char
rank_to_char(Rank rank)
{
	assert(rank_is_in_bounds(rank));
	return rank + '1';
}

Rank
char_to_rank(char c)
{
	if (isdigit(c)) {
		Rank rank = c - '1';
		assert(rank_is_in_bounds(rank));
		return rank;
	} else {
		return RANK_NONE;
	}
}

const Rank RANK_NONE = (1 << 8) - 1;

// COORD
// =====

Coord
coord_new(File file, Rank rank)
{
	assert(file_is_in_bounds(file));
	assert(rank_is_in_bounds(rank));
	return (rank << 3) | file;
}

bool
coord_is_in_bounds(Coord coord)
{
	return coord < BOARD_NUM_SQUARES;
}

File
coord_file(Coord coord)
{
	assert(coord_is_in_bounds(coord));
	return coord & 0x7;
}

Rank
coord_rank(Coord coord)
{
	assert(coord_is_in_bounds(coord));
	return coord >> 3;
}

const Coord COORD_NONE = (1 << 8) - 1;
