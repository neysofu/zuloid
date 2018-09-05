#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "coord.h"
#include "dir.h"
#include "piece.h"

// A move from one square to another.
typedef uint32_t Move;

// Creates a move from one square to another. 'promotion' is ignored if the move
// is not actually a promotion.
Move
move_new(Coord source, Coord destination, enum Piece promotion);

Coord
move_source(Move move);

Coord
move_target(Move move);

enum Piece
move_piece(Move move);

enum Piece
move_promotion(Move move);

enum Dir
move_dir(Move move);

char *
move_to_str(Move move, char *str);

Move
str_to_move(char *str);

extern const Move MOVE_NONE;
