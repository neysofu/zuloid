/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_CHESS_MOVE_H
#define Z64C_CHESS_MOVE_H

#include "chess/coordinates.h"
#include "chess/piece_types.h"
#include <stdbool.h>
#include <stdint.h>

struct Move
{
	Bitboard source;
	Bitboard target;
	Piece piece;
	bool is_capture;
	bool is_en_passant;
	bool is_promotion;
};

char *
string_new_from_move(struct Move *mv);

struct Move
move_set_from_string(struct Move *mv, const char *s);

#endif
