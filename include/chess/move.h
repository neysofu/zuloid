/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_CHESS_MOVE_H
#define Z64C_CHESS_MOVE_H

#include "chess/coordinates.h"
#include "chess/pieces.h"
#include "chess/position.h"
#include <stdbool.h>
#include <stdint.h>

/* A reversible chess move. */
struct Move
{
	Square source;
	Square target;
	enum PieceType promotion;
	enum PieceType capture;
};

char *
move_to_string(struct Move move);

struct Move
string_to_move(const char *string);

void
position_do_move(struct Position *position, struct Move *move);

void
position_undo_move(struct Position *position, const struct Move *move);

#endif
