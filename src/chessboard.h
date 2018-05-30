#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "color.h"
#include "move.h"
#include "castling.h"
#include "square.h"

#define CHESSBOARD_SIDE_LENGHT 8
#define FIFTY 50

struct Chessboard {
	// The first index is ranks and the second one is files, not the other way
	// around!
	struct Square squares[CHESSBOARD_SIDE_LENGHT][CHESSBOARD_SIDE_LENGHT];
	struct CastlingRights castling_rights;
	struct Coordinates* en_passant_target;
	enum Color active_color;
	uint8_t half_moves;
	uint64_t history_by_hash[FIFTY];
	uint8_t num_pieces;
	//struct LegalMovesCache stalemate_detector_white;
	//struct LegalMovesCache stalemate_detector_black;
	struct ColorSpecificInfo white;
	struct ColorSpecificInfo black;
};

struct Square *
cb_square(struct Chessboard *cb, const struct Coordinates coord);

const struct Square *
cb_square_const(const struct Chessboard *cb, const struct Coordinates coord);

bool
cb_is_move_capture(const struct Chessboard *cb, const struct Move mv);

bool
cb_is_threefold_repetition(const struct Chessboard *cb);

void
cb_update_history(struct Chessboard *cb);

void
cb_update_en_passant_file(struct Chessboard *cb);

void
cb_remove_en_passant_file(struct Chessboard *cb);

void
cb_promote_pawn(struct Chessboard *cb, const struct Move mv);

uint64_t
cb_hash(const struct Chessboard *cb);

bool
cb_is_stalemate(const struct Chessboard *cb, const struct Move mv);

bool
cb_is_check(const struct Chessboard *cb, const struct Move mv);

bool
cb_is_move_legal(const struct Chessboard *cb, const struct Move mv);

bool
cb_is_move_promotion(const struct Chessboard *cb, const struct Move mv);

bool
cb_is_move_pseudolegal_pawn(const struct Chessboard *cb, const struct Move mv);

bool
cb_is_move_pseudolegal_knight(const struct Chessboard *cb, const struct Move mv);

bool
cb_is_move_pseudolegal_bishop(const struct Chessboard *cb, const struct Move mv);

bool
cb_is_move_pseudolegal_rook(const struct Chessboard *cb, const struct Move mv);

bool
cb_is_move_pseudolegal_bishop(const struct Chessboard *cb, const struct Move mv);

bool
cb_is_move_pseudolegal_rook(const struct Chessboard *cb, const struct Move mv);

bool
cb_is_move_pseudolegal_queen(const struct Chessboard *cb, const struct Move mv);

bool
cb_is_move_pseudolegal_king(const struct Chessboard *cb, const struct Move mv);

bool
cb_is_move_en_passant(const struct Chessboard *cb, const struct Move mv);

void
cb_move(struct Chessboard *cb, const struct Move mv);

bool
cb_is_gameover(const struct Chessboard *cb);

bool
mv_is_obstructed(const struct Chessboard *cb, const struct Move mv);

extern const struct Chessboard *CB_INIT;
