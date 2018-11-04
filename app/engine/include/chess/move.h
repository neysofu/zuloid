/**
 * @file move.h
 * @brief Utilities for integer representation of chess moves.
 */

#pragma once

#include "chess/board.h"
#include "chess/coord.h"
#include "chess/dir.h"
#include "chess/piece.h"
#include <stdbool.h>
#include <stdint.h>

// A move from one square to another.
// TODO: turn it into a uint16_t.
typedef uint32_t Move;

// Creates a move from one square to another. 'promotion' is ignored if the move
// is not actually a promotion.
Move
move_new(Coord source, Coord destination, enum Piece promotion);

/// @brief Returns the source square of the move @p move.
Coord
move_source(Move move);

/// @brief Returns the target square of the move @p move.
Coord
move_target(Move move);

enum Piece
move_piece(Move move);

/// @brief Returns the promotion piece type of ch
enum Piece
move_promotion(Move move);

bool
move_is_capture(Move move, struct Board *board);

bool
move_is_en_passant(Move move, struct Board *board);

enum Dir
move_dir(Move move);

char *
move_to_str(Move move, char *str);

Move
str_to_move(char *str);

extern const Move MOVE_NONE;
