#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "board.h"
#include "coord.h"
#include "dir.h"
#include "piece.h"
#include "result.h"

// A move from one square to another.
typedef uint_fast32_t Move;

// Create a move from one square to another. 'promotion' is ignored if the move
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

char *
move_to_str(Move move, char *str);

Move
str_to_move(char *str);

bool
move_is_legal(Move move, struct Board *board);
bool
move_is_castling(Move move, struct Board *board);
bool
move_is_promotion(Move move, struct Board *board);
bool
move_is_reversible(Move move, struct Board *board);
bool
move_is_en_passant(Move move, struct Board *board);
bool
move_is_capture(Move move, struct Board *board);
bool
move_is_self_harm(Move move, struct Board *board);
bool
move_triggers_attack(Move move, Coord coord, struct Board *board);
bool
move_triggers_draw_by_repetition(Move move, struct Board *board);
bool
move_triggers_stalemate(Move move, struct Board *board);
bool
move_triggers_check(Move move, struct Board *board);
bool
move_triggers_checkmate(Move move, struct Board *board);

// Make a move without first checking its legality and return the game result,
// or 'RESULT_NONE' if the game is not over.
struct Result
board_push(struct Board *board, Move move);

extern const Move MOVE_NONE;
