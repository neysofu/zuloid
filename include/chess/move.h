#ifndef ZORRO_CHESS_MOVE_H
#define ZORRO_CHESS_MOVE_H

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
position_do_move_and_flip(struct Position *pos, struct Move *mv);

void
position_undo_move(struct Position *pos, const struct Move *mv);
void
position_undo_move_and_flip(struct Position *pos, const struct Move *mv);

int
move_file_diff(struct Move *mv);

int
move_rank_diff(struct Move *mv);

Bitboard
move_ray(struct Move *mv);

#endif
