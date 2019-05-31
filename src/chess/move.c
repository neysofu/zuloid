/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chess/move.h"
#include "chess/coordinates.h"
#include "chess/pieces.h"
#include "chess/position.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//
// bool
// move_is_capture(Move move, struct Board *board)
//{
//	return board->bb_occupancy & bb_coord(move_target(move));
//}
//
// bool
// move_is_en_passant(Move move, struct Board *board)
//{
//	return false; // TODO
//}
//
// char *
// move_to_str(Move move)
//{
//	char *str = malloc(5);
//	buffer[0] = FILE_TO_CHAR(COORD_FILE(MOVE_SOURCE(move)));
//	buffer[1] = RANK_TO_CHAR(COORD_RANK(MOVE_SOURCE(move)));
//	buffer[2] = FILE_TO_CHAR(COORD_FILE(MOVE_TARGET(move)));
//	buffer[3] = RANK_TO_CHAR(COORD_RANK(MOVE_TARGET(move)));
//	if (MOVE_PROMOTION(move) == PIECE_NONE) {
//		buffer[4] = '\0';
//	} else {
//		buffer[4] = piece_to_char(MOVE_PROMOTION(move));
//		buffer[5] = '\0';
//	}
//	return buffer;
//}
//
// Move
// str_to_move(const char *str)
//{
//	assert(str);
//	if (strlen(str) < 4) {
//		return MOVE_NONE;
//	}
//	Coord source = COORD(CHAR_TO_FILE(str[0]), CHAR_TO_RANK(str[1]));
//	Coord target = COORD(CHAR_TO_FILE(str[2]), CHAR_TO_RANK(str[3]));
//	enum Piece promotion = isalpha(str[4]) ? toupper(str[4]) : PIECE_QUEEN;
//	return MOVE(source, target, promotion);
//}
//
// bitboard
// move_ray(Move move)
//{
//	Coord source = move_source(move);
//	Coord target = move_target(move);
//	enum Dir dir = MOVE_DIR(move);
//	assert(coord <= COORD_MAX);
//	assert(coord <= COORD_MAX);
//	assert(dir == DIR_HORIZONTAL || dir == DIR_VERTICAL || dir ==
// DIR_DIAGONAL); 	bitboard ray = (COORD_TO_BITBOARD(source) - 1) ^
//(COORD_TO_BITBOARD(target) - 1);
//	// void *(filters) = { bb_rank, bb_file, bb_bishop_threats };
//	return ray & (ray - 1); // & filters[dir](source);
//}

void
position_do_move(struct Position *position, struct Move *move)
{
	assert(position);
	assert(move);
	move->capture = position_piece_at_square(position, move->target).type;
}

void
position_undo_move(struct Position *position, const struct Move *move)
{
	assert(position);
	assert(move);
	/* TODO */
}
