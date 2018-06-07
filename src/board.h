#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "color.h"
#include "castling.h"
#include "square.h"

#define BOARD_SIDE_LENGTH 8
#define MAX_HALF_MOVES 75

struct Board {
	struct Square squares[BOARD_SIDE_LENGTH][BOARD_SIDE_LENGTH];
	struct CastlingRights castling_rights;
	struct Coord en_passant_target;
	enum Color active_color;
	uint8_t half_moves;
	uint64_t history_by_hash[MAX_HALF_MOVES];
	uint8_t num_pieces;
};

struct Board *
board_new(struct Board *board);

void
board_drop(struct Board *board);

struct Square *
board_square(struct Board *board, const struct Coord coord);

const struct Square *
board_square_const(const struct Board *board, const struct Coord coord);

void
board_update_history(struct Board *board);

uint64_t
board_hash(const struct Board *board);

// Checks if the game is over and is yes what the result is.
bool
board_is_gameover(const struct Board *board);

extern const struct Board BOARD_STARTPOS;
