#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "bitboards.h"
#include "board.h"
#include "coord.h"
#include "move.h"
#include "mover.h"
#include "piece.h"
#include "result.h"
#include "utils.h"

void
mover_set_up(struct Mover *mover,
		      struct Board *board,
			  Move move) {
	mover->board = board;
	mover->move = move;
	mover->source = move_source(move);
	mover->target = move_target(move);
	//mover->piece = piece;
	mover->dir = move_dir(move);
	mover->active_color = board_active_color(board);
	mover->is_capture = board->bb_occupancy & coord_to_bb(mover->source);
	mover->is_castling = 1; // TODO
	mover->is_en_passant = mover->target ==
						   board_en_passant_coord(mover->board);
	mover->is_promotion = coord_rank(move_target(move)) ==
		   color_promoting_rank(board_active_color(board));
	mover->num_halfmoves = board_num_half_moves(board);
	mover->castling_rights = board_castling_rights(board);
}

bool
mover_approves_pseudolegal_move_by_pawn(struct Mover *mover) {
	bool is_on_home_rank = coord_rank(mover->source) ==
		                   color_pawn_rank(mover->active_color);
	switch (move_dir(mover->move)) {
		case DIR_DIAGONAL:
			return (mover->is_capture || mover->is_en_passant) &&
				   (coord_rank(mover->target) -
					coord_rank(mover->source) ==
					mover->active_color ? 1 : -1);
		case DIR_VERTICAL:
			return !mover->is_capture &&
				   !(mover->board->bb_occupancy &
					 (coord_to_bb(mover->source) +
					  mover->active_color ? mover->source << 4 :
					                        mover->source >> 4)) &&
				   (coord_rank(mover->target) -
					coord_rank(mover->source) <=
					mover->active_color ? 1 + is_on_home_rank :
					                      -(1 + is_on_home_rank));
		default:
			return false;
	}
}

bool
mover_approves_pseudolegal_move_by_knight(struct Mover *mover) {
	return bb_knight_attacks(mover->source) & coord_to_bb(mover->target);
}

bool
mover_approves_pseudolegal_move_by_bishop(struct Mover *mover) {
	uint64_t bb_source_mask = coord_to_bb(mover->source) - 1;
	uint64_t bb_target_mask = coord_to_bb(mover->target) - 1;
	if (coord_file(mover->source) > coord_file(mover->target)) {
		return mover->board->bb_occupancy &
			   bb_diagonal_main(mover->source) &
			   (bb_source_mask ^ bb_target_mask);
	} else if (mover->dir == DIR_VERTICAL) {
		return mover->board->bb_occupancy &
			   bb_diagonal_anti(mover->source) &
			   (bb_source_mask ^ bb_target_mask);
	} else {
		return false;
	}
}

bool
mover_approves_pseudolegal_move_by_rook(struct Mover *mover) {
	uint64_t bb_source_mask = coord_to_bb(mover->source) - 1;
	uint64_t bb_target_mask = coord_to_bb(mover->target) - 1;
	if (mover->dir == DIR_HORIZONTAL) {
		return mover->board->bb_occupancy &
			   bb_rank(mover->source) &
			   (bb_source_mask ^ bb_target_mask);
	} else if (mover->dir == DIR_VERTICAL) {
		return mover->board->bb_occupancy &
			   bb_file(mover->source) &
			   (bb_source_mask ^ bb_target_mask);
	} else {
		return false;
	}
}

bool
mover_approves_pseudolegal_castling_move(struct Mover *mover) {
	//return board->bb_pieces[PIECE_ROOK] & board->bb_colors(board, move_source(move))->piece == PIECE_KING &&
	//	   abs(move_source(move).file - move_target(move).file) == 2;
	return true; // TODO
}

bool
mover_approves_pseudolegal_move_by_king(struct Mover *mover) {
	bool is_single_move = bb_king_attacks(mover->source) &
		                  coord_to_bb(mover->target);
	return is_single_move ||
		   mover_approves_pseudolegal_castling_move(mover);
}

bool
mover_approves_pseudolegal_move_by_queen(struct Mover *mover) {
	return mover_approves_pseudolegal_move_by_bishop(mover) ||
		   mover_approves_pseudolegal_move_by_rook(mover);
}

bool
mover_approves_pseudolegal_move(struct Mover *mover) {
	bool is_self_capture = !((mover->board->bb_colors &
			                  coord_to_bb(mover->source)) ^
			                 mover->active_color);
	bool is_own_move = mover->board->bb_occupancy & coord_to_bb(mover->source);
	if (is_self_capture || !is_own_move) {
		return false;
	}
	switch (mover->piece) {
		case PIECE_PAWN:
			return mover_approves_pseudolegal_move_by_pawn(mover);
		case PIECE_KNIGHT:
			return mover_approves_pseudolegal_move_by_knight(mover);
		case PIECE_BISHOP:
			return mover_approves_pseudolegal_move_by_bishop(mover);
		case PIECE_ROOK:
			return mover_approves_pseudolegal_move_by_rook(mover);
		case PIECE_KING:
			return mover_approves_pseudolegal_move_by_king(mover);
		case PIECE_QUEEN:
			return mover_approves_pseudolegal_move_by_queen(mover);
		default:
			return false;
	}
}

bool
mover_approves_legal_move(struct Mover *mover) {
	return true;
}

struct Result
mover_push_pseudolegal_move(struct Mover *mover) {
	if (mover->is_capture || mover->piece == PIECE_PAWN) {
		board_increment_half_moves(mover->board);
	} else {
		board_reset_half_moves(mover->board);
	}
	if (mover->board->bb_pieces[PIECE_KING] & coord_to_bb(mover->target)) {
		return (struct Result) {
			.winner = mover->active_color,
			.termination = TERMINATION_CHECKMATE,
		};
	}
	//if (mover->is_en_passant) {
	//	board->bb_occupancy &= mover->target;
	//	board->bb_pieces[PIECE_PAWN] &= mover->target;
	//} else if (mover->is_promotion) {
	//	board->bb_pieces[mover->promotion] |= coord_to_bb(mover->target);
	//} else if (mover->is_castling) {
	//	File rook_source_file;
	//	File rook_target_file;
	//	if (coord_file(mover->source) == 2) {
	//		rook_source_file = 0;
	//		rook_target_file = 3;
	//	} else {
	//		rook_source_file = 7;
	//		rook_target_file = 5;
	//	}
	//	Coord rook = coord_new(rook_source_file, move_source(move).rank);
	//	*board_square(board, rook) = SQUARE_NONE;
	//	rook.file = rook_target_file;
	//	board_square(mover->board, rook)->piece = PIECE_ROOK;
	//	board_square(board, rook)->color = board->active_color;
	//}
	mover->board->bb_occupancy &= !coord_to_bb(mover->source);
	mover->board->bb_occupancy |= coord_to_bb(mover->target);
	mover->board->bb_colors |= coord_to_bb(mover->target);
	mover->board->game_state = color_other(mover->active_color);
	if (board_num_half_moves(mover->board) >= BOARD_MAX_HALF_MOVES) {
		return (struct Result) {
			.winner = COLOR_NONE,
			.termination = TERMINATION_NO_CAPTURE,
		};
	}
	return RESULT_NONE;
}

struct Result
mover_push_legal_move(struct Mover *mover) {
	return RESULT_NONE;
}
