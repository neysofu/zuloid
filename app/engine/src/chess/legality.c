#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "bitboards.h"
#include "chess/board.h"
#include "chess/coord.h"
#include "chess/move.h"
#include "chess/legality.h"
#include "chess/piece.h"
#include "chess/result.h"
#include "utils.h"

bool
board_approves_pseudolegal_pawn_move(struct Board *board, Move move) {
	assert(board);
	Coord source = move_source(move);
	Coord target = move_target(move);
	uint64_t source_mask = bb_coord(source);
	enum Color active_color = board_active_color(board);
	bool is_on_home_rank = coord_rank(source) ==
		                   color_pawn_rank(active_color);
	bool is_capture = move_is_capture(move, board);
	int8_t rank_diff = coord_rank(target) - coord_rank(source);
	switch (move_dir(move)) {
		case DIR_DIAGONAL:
			return (is_capture || move_is_en_passant(move, board)) &&
				   (abs(rank_diff) == 1);
		case DIR_VERTICAL:
			return !is_capture &&
				   !(board->bb_occupancy &
					 (active_color ? source_mask >> BOARD_SIDE_LENGTH :
					                 source_mask << BOARD_SIDE_LENGTH)) &&
				   (abs(rank_diff) <= 1 + is_on_home_rank) &&
				   (active_color ? rank_diff < 0 : rank_diff > 0);
		default:
			return false;
	}
}

bool
board_approves_pseudolegal_knight_move(struct Board *board, Move move) {
	assert(board);
	return bb_knight_threats(move_source(move)) & bb_coord(move_target(move));
}

bool
board_approves_pseudolegal_sliding_move(struct Board *board, Move move) {
	assert(board);
	uint64_t ray = bb_ray(move_source(move),
						  move_target(move),
						  dir_from_to(move_source(move), move_target(move)));
	return !(ray & board->bb_occupancy);
}

bool
board_approves_pseudolegal_castling_move(struct Board *board, Move move) {
	//return board->bb_pieces[PIECE_ROOK] & board->bb_colors(board, move_source(move))->piece == PIECE_KING &&
	//	   abs(move_source(move).file - move_target(move).file) == 2;
	return false; // TODO
}

bool
board_approves_pseudolegal_king_move(struct Board *board, Move move) {
	bool is_single_move = bb_king_threats(move_source(move)) &
		                  bb_coord(move_target(move));
	return is_single_move ||
		   board_approves_pseudolegal_castling_move(board, move);
}

bool
board_approves_pseudolegal_move(struct Board *board,
								Move move,
								enum Piece piece) {
	enum Color source_color = (board->bb_colors &
			                   bb_coord(move_source(move))) > COLOR_WHITE;
	enum Color target_color = (board->bb_colors &
			                   bb_coord(move_target(move))) > COLOR_WHITE;
	if (board_active_color(board) != source_color ||
		(board_active_color(board) == target_color &
		 move_is_capture(move, board))) {
		return false;
	}
	switch (piece) {
		case PIECE_PAWN:
			return board_approves_pseudolegal_pawn_move(board, move);
		case PIECE_KNIGHT:
			return board_approves_pseudolegal_knight_move(board, move);
		case PIECE_KING:
			return board_approves_pseudolegal_king_move(board, move);
		case PIECE_NONE:
			return false;
		default:
			assert(piece == PIECE_BISHOP ||
				   piece == PIECE_ROOK ||
				   piece == PIECE_QUEEN);
			return board_approves_pseudolegal_sliding_move(board, move);
	}
	assert(false);
}

bool
board_approves_legal_move(struct Board *board,
						  Move move,
						  enum Piece piece) {
	return board_approves_pseudolegal_move(board, move, piece);
	// TODO
}

struct Result
board_push_pseudolegal_move(struct Board *board,
							Move move,
							enum Piece piece) {
	enum Color active_color = board_active_color(board);
	if (move_is_capture(move, board) || piece == PIECE_PAWN) {
		//board_increment_half_moves(board);
	} else {
		//board_reset_half_moves(board);
	}
	bool is_king_capture = board->bb_pieces[PIECE_KING] &
						   bb_coord(move_target(move));
	if (is_king_capture) {
		return (struct Result) {
			.winner = active_color,
			.termination = TERMINATION_CHECKMATE,
		};
	}
	if (false) {
		//board->bb_occupancy &= board->target;
		//board->bb_pieces[PIECE_PAWN] &= board->target;
	} else if (piece == PIECE_PAWN) {
		//board->bb_pieces[board->promotion] |= bb_coord(board->target);
	} else if (false) { // TODO
	//	File rook_source_file;
	//	File rook_target_file;
	//	if (coord_file(board->source) == 2) {
	//		rook_source_file = 0;
	//		rook_target_file = 3;
	//	} else {
	//		rook_source_file = 7;
	//		rook_target_file = 5;
	//	}
	//	Coord rook = coord_new(rook_source_file, move_source(move).rank);
	//	*board_square(board, rook) = SQUARE_NONE;
	//	rook.file = rook_target_file;
	//	board_square(board->board, rook)->piece = PIECE_ROOK;
	//	board_square(board, rook)->color = board->active_color;
	}
	uint64_t source_mask = ~bb_coord(move_source(move));
	uint64_t target_mask = bb_coord(move_target(move));
	board->bb_pieces[piece] &= source_mask;
	board->bb_pieces[piece] |= target_mask;
	board->bb_occupancy &= source_mask;
	board->bb_occupancy |= target_mask;
	board->bb_colors = (board->bb_colors & ~target_mask) |
					   ((uint64_t)(active_color) << (uint64_t)(move_target(move)));
	board_toggle_active_color(board);
	if (board_num_half_moves(board) >= BOARD_MAX_HALF_MOVES) {
		return (struct Result) {
			.winner = COLOR_NONE,
			.termination = TERMINATION_NO_CAPTURE,
		};
	}
	return RESULT_NONE;
}

struct Result
board_push_legal_move(struct Board *board,
				      Move move,
					  enum Piece piece) {
	return board_push_pseudolegal_move(board, move, piece); // TODO
}
