#pragma once

#include <stdbool.h>

#include "board.h"
#include "coord.h"
#include "dir.h"
#include "piece.h"

struct Move {
	struct Coord source;
	struct Coord target;
	enum Dir dir;
	// Ignored if the move is not actually a promotion; a common pattern is to
	// set this field to 'PIECE_NONE' in all the other cases.
	enum Piece promotion;
};

char *
move_to_str(const struct Move move, char *str);

struct Move
str_to_move(const char *str);

bool
move_is_legal(const struct Move move, const struct Board *board);

bool
move_is_obstructed(const struct Move move, const struct Board *board);

bool
move_is_promotion(const struct Move move, const struct Board *board);

bool
move_is_en_passant(const struct Move move, const struct Board *board);

bool
move_is_capture(const struct Move move, const struct Board *board);

bool
move_is_self_harm(const struct Move move, const struct Board *board);

bool
move_triggers_attack(const struct Move move, const struct Coord coord, const struct Board *board);

bool
move_triggers_draw_by_repetition(const struct Board *board);

bool
move_triggers_stalemate(const struct Move move, const struct Board *board);

bool
move_triggers_check(const struct Move move, const struct Board *board);

bool
move_triggers_checkmate(const struct Move move, const struct Board *board);

void
board_move(struct Board *board, const struct Move move);

extern const struct Move MOVE_NONE;
