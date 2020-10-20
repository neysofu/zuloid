/* SPDX-License-Identifier: GPL-3.0-only */

#ifndef ZULOID_CHESS_MOVE_H
#define ZULOID_CHESS_MOVE_H

#include "chess/coordinates.h"
#include "chess/pieces.h"
#include "chess/position.h"
#include <stdbool.h>
#include <stdint.h>

enum
{
	MOVE_STRING_MAX_LENGTH = 6
};

/* A reversible chess move. */
struct Move
{
	Square source;
	Square target;
	enum PieceType promotion;
	enum PieceType capture;
	bool castling;
	int castling_side;
};

const struct Move MOVE_IDENTITY;

bool
moves_eq(const struct Move *m1, const struct Move *m2);

bool
string_represents_coordinate_notation_move(const char *str);

size_t
move_to_string(struct Move mv, char *buf);

size_t
string_to_move(const char *str, struct Move *mv);

bool
position_check_pseudolegality(struct Board *pos, struct Move *mv);

void
position_do_move(struct Board *pos, struct Move *mv);
void
position_do_move_and_flip(struct Board *pos, struct Move *mv);

void
position_undo_move(struct Board *pos, const struct Move *mv);
void
position_undo_move_and_flip(struct Board *pos, const struct Move *mv);

int
move_file_diff(struct Move *mv);

int
move_rank_diff(struct Move *mv);

Bitboard
move_ray(struct Move *mv);

#endif
