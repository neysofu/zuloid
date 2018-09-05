#pragma once

#include "board.h"
#include "move.h"
#include "result.h"

struct Mover {
	struct Board *board;
	Move move;
	Coord source;
	Coord target;
	enum Piece piece;
	enum Dir dir;
	enum Color active_color;
	bool is_capture;
	bool is_castling;
	bool is_en_passant;
	bool is_promotion;
	uint_fast8_t num_halfmoves;
	uint_fast8_t castling_rights;
};

void
mover_set_up(struct Mover *mover, struct Board *board, Move move);

bool
mover_approves_pseudolegal_move(struct Mover *mover);

bool
mover_approves_legal_move(struct Mover *mover);

struct Result
mover_push_pseudolegal_move(struct Mover *mover);

struct Result
mover_push_legal_move(struct Mover *mover);
