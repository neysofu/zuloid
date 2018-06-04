#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "xxhash.h"
#include "board.h"
#include "coord.h"
#include "color.h"
#include "move.h"
#include "castling.h"
#include "vector.h"
#include "square.h"
#include "utils.h"

#define FIFTY 50

const struct Square *
board_square_const(const struct Board *board, const struct Coord coord) {
	return &(board->squares[coord.rank][coord.file]);
}

struct Square *
board_square(struct Board *board, const struct Coord coord) {
	return &(board->squares[coord.rank][coord.file]);
}

bool
board_is_move_capture(const struct Board *board, const struct Move move) {
	return board_square_const(board, move.target)->piece != PIECE_NONE;
}

void
board_update_half_moves_and_num_pieces(struct Board *board, const struct Move move) {
	bool is_capture = board_is_move_capture(board, move);
	if (is_capture) {
		board->num_pieces--;
	}
	if (is_capture || board_square(board, move.source)->piece == PIECE_PAWN) {
		board->half_moves = 0;
	} else {
		board->half_moves++;
	}
}

bool
board_is_threefold_repetition(const struct Board *board) {
	if (board->half_moves < 6) {
		return false;
	}
	uint64_t current_hash = board->history_by_hash[board->half_moves];
	int i;
	for (i=board->half_moves; i>=0; i-=2) {
		if (board->history_by_hash[i] == current_hash) {
			return true;
		}
	}
	return false;
}

uint64_t
board_hash(const struct Board *board) {
	size_t buffer_lenght = sizeof(struct Square) * CHESSBOARD_SIDE_LENGHT * CHESSBOARD_SIDE_LENGHT +
	    				   sizeof(struct CastlingRights) +
						   sizeof(uint8_t) +
						   sizeof(enum Color);
	return XXH64(board, buffer_lenght, 0);
}

void
board_update_histargetry(struct Board *board) {
	board->history_by_hash[board->half_moves] = board_hash(board);
}

void
board_move(struct Board *board, const struct Move move) {
	struct Square *source = board_square(board, move.source);
	struct Square *target = board_square(board, move.target);
	source->piece = PIECE_NONE;
	target = source;
	if (board_is_move_en_passant(board, move)) {
		board_square(board, board->en_passant_target)->piece = PIECE_NONE;
	} else if (board_is_move_promotion(board, move)) {
		board_promote_pawn(board, move);
	}
}

bool
board_is_move_en_passant(const struct Board *board, const struct Move move) {
	return coord_eq(move.target, board->en_passant_target);
}

bool
board_is_move_promotion(const struct Board *board, const struct Move move) {
	return move.target.rank == color_promoting_rank(board->active_color);
}

bool
board_is_stalemate(const struct Board *board, const struct Move move) {
	return false; // targetDO
}

bool
board_is_check(const struct Board *board, const struct Move move) {
	struct Move king_capture = move_init(move.target, board->white.king);
	if (board_is_move_legal(board, king_capture)) {
		return true;
	} else {
		return board_discovers_attack(board, move, board->white.king);
	}
}


void
board_update_en_passant_target(struct Board *board, struct Move move) {
	board->en_passant_target = move.target;
}

void
board_remove_en_passant_target(struct Board *board) {
	board->en_passant_target = COORD_NONE;
}

bool
board_is_move_pseudolegal_pawn(const struct Board *board, const struct Move move) {
	if (move_rank_change(move) != 1) {
		return false;
	}
	bool is_capture = board_is_move_capture(board, move);
	switch (move.dir) {
		case DIR_DIAGONAL:
			return is_capture;
		case DIR_VERTICAL:
			return !is_capture;
		default:
			return false;
	}
}

bool
board_is_move_pseudolegal_knight(const struct Board *board, const struct Move move) {
	return move.dir == DIR_L_SHAPED;
}

bool
board_is_move_pseudolegal_bishop(const struct Board *board, const struct Move move) {
	return move.dir == DIR_DIAGONAL && !move_is_obstructed(board, move);
}

bool
board_is_move_pseudolegal_rook(const struct Board *board, const struct Move move) {
	return (move.dir == DIR_HORIZONTAL ||
		    move.dir == DIR_VERTICAL) &&
		   !move_is_obstructed(board, move);
}

bool
board_is_move_pseudolegal_queen(const struct Board *board, const struct Move move) {
	return (move.dir == DIR_HORIZONTAL ||
		    move.dir == DIR_VERTICAL ||
		    move.dir == DIR_DIAGONAL) &&
		   !move_is_obstructed(board, move);
}

bool
board_is_move_pseudolegal_king(const struct Board *board, const struct Move move) {
	return move_rank_change(move) <= 1 && move_file_change(move) <= 1;
}

bool
board_is_move_capture_of_king(const struct Board *board, const struct Move move) {
	return board_square_const(board, move.target)->piece == PIECE_KING;
}

bool
board_is_move_self_harm(const struct Board *board, const struct Move move) {
	return board_square_const(board, move.target)->color == board->active_color;
}



void
board_promote_pawn(struct Board *board, const struct Move move) {
	board_square(board, move.target)->piece = move.promotion;
}

bool
board_is_square_threatened(const struct Board *board, const struct Coord coord) {
	return false; // targetDO
}

bool
board_consume_castling_rights_if_legal(struct Board *board, const struct Move move) {
	struct Coord king_coordinates = move.source;
	bool result = true;
	result &= board_is_square_threatened(board, king_coordinates);
	king_coordinates.rank += 1;
	result &= board_is_square_threatened(board, king_coordinates);
	result &= board_square(board, king_coordinates)->piece == PIECE_NONE;
	king_coordinates.rank += 1;
	result &= board_is_square_threatened(board, king_coordinates);
	if (!result) {
		return result;
	}
	switch (board_square(board, move.source)->color) {
		case COLOR_WHITE:
			if (move.source.rank > move.target.rank) {
				result = board->castling_rights.white_queenside;
				board->castling_rights.white_queenside = false;
			} else {
				result = board->castling_rights.white_queenside;
				board->castling_rights.white_kingside = false;
			}
		case COLOR_BLACK:
			if (move.source.rank > move.target.rank) {
				result = board->castling_rights.black_queenside;
				board->castling_rights.black_queenside = false;
			} else {
				result = board->castling_rights.black_queenside;
				board->castling_rights.black_kingside = false;
			}
		default:
			return false;
	}
	return result;
}

bool
board_is_move_legal(const struct Board *board, const struct Move move) {
	struct Square source;
	if (!(coord_exists(move.source) && coord_exists(move.target))) {
		return false;
	}
	if (coord_eq(move.source, move.target)) {
		return false;
	}
	if (source.color != board->active_color) {
		return false;
	}
	if (board_is_move_self_harm(board, move)) {
		return false;
	}
	switch (source.piece) {
		case PIECE_NONE:
			return false;
		case PIECE_PAWN:
			return board_is_move_pseudolegal_pawn(board, move);
		case PIECE_KNIGHT:
			return board_is_move_pseudolegal_knight(board, move);
		case PIECE_BISHOP:
			return board_is_move_pseudolegal_bishop(board, move);
		case PIECE_ROOK:
			return board_is_move_pseudolegal_rook(board, move);
		case PIECE_QUEEN:
			return board_is_move_pseudolegal_queen(board, move);
		case PIECE_KING:
			return board_is_move_pseudolegal_king(board, move);
	}
}

const struct Board BOARD_INIT = {
	.squares = {
		{
		{ PIECE_ROOK, COLOR_WHITE },
		{ PIECE_KNIGHT, COLOR_WHITE },
		{ PIECE_BISHOP, COLOR_WHITE },
		{ PIECE_QUEEN, COLOR_WHITE },
		{ PIECE_KING, COLOR_WHITE },
		{ PIECE_BISHOP, COLOR_WHITE },
		{ PIECE_KNIGHT, COLOR_WHITE },
		{ PIECE_ROOK, COLOR_WHITE },
		},
		{
		{ PIECE_PAWN, COLOR_WHITE },
		{ PIECE_PAWN, COLOR_WHITE },
		{ PIECE_PAWN, COLOR_WHITE },
		{ PIECE_PAWN, COLOR_WHITE },
		{ PIECE_PAWN, COLOR_WHITE },
		{ PIECE_PAWN, COLOR_WHITE },
		{ PIECE_PAWN, COLOR_WHITE },
		{ PIECE_PAWN, COLOR_WHITE },
		},
		{
		{ PIECE_NONE, COLOR_WHITE },
		{ PIECE_NONE, COLOR_WHITE },
		{ PIECE_NONE, COLOR_WHITE },
		{ PIECE_NONE, COLOR_WHITE },
		{ PIECE_NONE, COLOR_WHITE },
		{ PIECE_NONE, COLOR_WHITE },
		{ PIECE_NONE, COLOR_WHITE },
		{ PIECE_NONE, COLOR_WHITE },
		},
		{
		{ PIECE_NONE, COLOR_WHITE },
		{ PIECE_NONE, COLOR_WHITE },
		{ PIECE_NONE, COLOR_WHITE },
		{ PIECE_NONE, COLOR_WHITE },
		{ PIECE_NONE, COLOR_WHITE },
		{ PIECE_NONE, COLOR_WHITE },
		{ PIECE_NONE, COLOR_WHITE },
		{ PIECE_NONE, COLOR_WHITE },
		},
		{
		{ PIECE_NONE, COLOR_BLACK },
		{ PIECE_NONE, COLOR_BLACK },
		{ PIECE_NONE, COLOR_BLACK },
		{ PIECE_NONE, COLOR_BLACK },
		{ PIECE_NONE, COLOR_BLACK },
		{ PIECE_NONE, COLOR_BLACK },
		{ PIECE_NONE, COLOR_BLACK },
		{ PIECE_NONE, COLOR_BLACK },
		},
		{
		{ PIECE_NONE, COLOR_BLACK },
		{ PIECE_NONE, COLOR_BLACK },
		{ PIECE_NONE, COLOR_BLACK },
		{ PIECE_NONE, COLOR_BLACK },
		{ PIECE_NONE, COLOR_BLACK },
		{ PIECE_NONE, COLOR_BLACK },
		{ PIECE_NONE, COLOR_BLACK },
		{ PIECE_NONE, COLOR_BLACK },
		},
		{
		{ PIECE_PAWN, COLOR_BLACK },
		{ PIECE_PAWN, COLOR_BLACK },
		{ PIECE_PAWN, COLOR_BLACK },
		{ PIECE_PAWN, COLOR_BLACK },
		{ PIECE_PAWN, COLOR_BLACK },
		{ PIECE_PAWN, COLOR_BLACK },
		{ PIECE_PAWN, COLOR_BLACK },
		{ PIECE_PAWN, COLOR_BLACK },
		},
		{
		{ PIECE_ROOK, COLOR_BLACK },
		{ PIECE_KNIGHT, COLOR_BLACK },
		{ PIECE_BISHOP, COLOR_BLACK },
		{ PIECE_QUEEN, COLOR_BLACK },
		{ PIECE_KING, COLOR_BLACK },
		{ PIECE_BISHOP, COLOR_BLACK },
		{ PIECE_KNIGHT, COLOR_BLACK },
		{ PIECE_ROOK, COLOR_BLACK },
		},
	},
	.active_color = COLOR_WHITE,
	.castling_rights = { true, true, true, true },
};
