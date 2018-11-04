/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "chess/move.h"
#include "bitboards.h"
#include "chess/board.h"
#include "chess/coord.h"
#include "chess/piece.h"
#include "chess/result.h"
#include "utils.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef Move uint_fast32_t;

Move
move_new(Coord source, Coord target, enum Piece promotion)
{
	return (promotion << 24) + (dir_from_to(source, target) << 16) +
	       (source << 8) + target;
}

Coord
move_source(Move move)
{
	return (move >> 8) & 0xff;
}

Coord
move_target(Move move)
{
	return move & 0xff;
}

enum Piece
move_promotion(Move move)
{
	return move >> 24;
}

enum Dir
move_dir(Move move)
{
	return (move >> 16) & 0xff;
}

bool
move_is_capture(Move move, struct Board* board)
{
	return board->bb_occupancy & bb_coord(move_target(move));
}

bool
move_is_en_passant(Move move, struct Board* board)
{
	return false; // TODO
}

char*
move_to_str(Move move, char* buffer)
{
	if (!buffer) {
		buffer = malloc(8);
		if (!buffer) {
			return NULL;
		}
	}
	buffer[0] = file_to_char(coord_file(move_source(move)));
	buffer[1] = rank_to_char(coord_rank(move_source(move)));
	buffer[2] = file_to_char(coord_file(move_target(move)));
	buffer[3] = rank_to_char(coord_rank(move_target(move)));
	if (move_promotion(move) == PIECE_NONE) {
		buffer[4] = '\0';
	} else {
		buffer[4] = piece_to_char(move_promotion(move));
		buffer[5] = '\0';
	}
	return buffer;
}

Move
str_to_move(char* str)
{
	assert(str);
	size_t len = strlen(str);
	if (len < 4) {
		return MOVE_NONE;
	}
	// FIXME
	File source_file = char_to_file(str[0]);
	Rank source_rank = char_to_rank(str[1]);
	File target_file = char_to_file(str[2]);
	Rank target_rank = char_to_rank(str[3]);
	if (!file_is_in_bounds(source_file) || !rank_is_in_bounds(source_rank) ||
	    !file_is_in_bounds(target_file) || !rank_is_in_bounds(target_rank)) {
		return MOVE_NONE;
	}
	Coord source = coord_new(source_file, source_rank);
	Coord target = coord_new(target_file, target_rank);
	bool is_promotion = len >= 5 && isalpha(str[4]);
	enum Piece promotion = is_promotion ? toupper(str[4]) : PIECE_QUEEN;
	return move_new(source, target, promotion);
}

const Move MOVE_NONE = UINT32_MAX;
