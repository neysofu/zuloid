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

size_t
move_to_string(struct Move mv, char *buf)
{
	assert(buf);
	size_t i = 0;
	buf[i++] = file_to_char(square_file(mv.source));
	buf[i++] = rank_to_char(square_rank(mv.source));
	buf[i++] = file_to_char(square_file(mv.target));
	buf[i++] = rank_to_char(square_rank(mv.target));
	if (mv.promotion) {
		buf[i++] = piece_to_char((struct Piece){ .type = mv.promotion });
	}
	return i;
}

size_t
string_to_move(const char *str, struct Move *mv)
{
	assert(str);
	assert(mv);
	if (strlen(str) < 4) {
		return 0;
	}
	mv->source = square_new(char_to_file(str[0]), char_to_rank(str[1]));
	mv->target = square_new(char_to_file(str[2]), char_to_rank(str[3]));
	mv->promotion = char_to_piece(str[4]).type;
	return 4;
}

void
position_do_move(struct Position *pos, struct Move *mv)
{
	assert(pos);
	assert(mv);
	mv->capture = position_piece_at_square(pos, mv->target).type;
}

void
position_undo_move(struct Position *position, const struct Move *move)
{
	assert(position);
	assert(move);
	/* TODO */
}
