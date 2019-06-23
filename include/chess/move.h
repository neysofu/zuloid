/* Copyright (c) Filippo Costa - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * PROPRIETARY AND CONFIDENTIAL */

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

size_t
move_to_string(struct Move mv, char *buf);

size_t
string_to_move(const char *str, struct Move *mv);

bool
position_check_pseudolegality(struct Position *pos, struct Move *mv);

void
position_do_move(struct Position *pos, struct Move *mv);

void
position_undo_move(struct Position *pos, const struct Move *mv);

int
move_file_diff(struct Move *mv);

int
move_rank_diff(struct Move *mv);

Bitboard
move_ray(struct Move *mv);

#endif
