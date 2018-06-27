#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "color.h"
#include "square.h"

#define BOARD_SIDE_LENGTH 8
#define MAX_HALF_MOVES 75
#define CASTLING_RIGHT_WHITE_KINGSIDE 0b1000
#define CASTLING_RIGHT_BLACK_KINGSIDE 0b0100
#define CASTLING_RIGHT_WHITE_QUEENSIDE 0b0010
#define CASTLING_RIGHT_BLACK_QUEENSIDE 0b0001

struct Board {
	struct Square squares[BOARD_SIDE_LENGTH][BOARD_SIDE_LENGTH];
	uint8_t castling_rights;
	File en_passant_file;
	enum Color active_color;
	uint8_t num_pieces;
	uint8_t num_half_moves;
};

struct Board *
board_new(struct Board *board);

void
board_drop(struct Board *board);

void
board_print(const struct Board *board);

struct Square *
board_square(const struct Board *board, const struct Coord coord);

struct Square *
board_en_passant_square(const struct Board *board);

extern const struct Board BOARD_STARTPOS;
