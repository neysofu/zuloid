#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "xxHash/xxhash.h"
#include "board.h"
#include "coord.h"
#include "color.h"
#include "move.h"
#include "castling.h"
#include "square.h"
#include "utils.h"

#define NUM_IRREVERSIBLE_MOVES_BEFORE_DRAW_CAN_BE_CLAIMED 50
#define NUM_IRREVERSIBLE_MOVES_BEFORE_DRAW_IS_FORCED 75

struct Board *
board_new(struct Board *board) {
	if (!board) {
		board = malloc(sizeof(struct Board));
		if (!board) {
			return NULL;
		}
	}
	*board = BOARD_STARTPOS;
	return board;
}

void
board_drop(struct Board *board) {
	free(board);
}

const struct Square *
board_square_const(const struct Board *board, const struct Coord coord) {
	return &(board->squares[coord.rank][coord.file]);
}

struct Square *
board_square(struct Board *board, const struct Coord coord) {
	return &(board->squares[coord.rank][coord.file]);
}

void
board_update_half_moves_and_num_pieces(struct Board *board, const struct Move move) {
	bool is_capture = move_is_capture(move, board);
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
	size_t buffer_lenght = sizeof(struct Square) * BOARD_SIDE_LENGTH * BOARD_SIDE_LENGTH +
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
	if (move_is_en_passant(move, board)) {
		board_square(board, board->en_passant_target)->piece = PIECE_NONE;
	} else if (move_is_promotion(move, board)) {
		board_square(board, move.target)->piece = move.promotion;
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
move_is_self_harm(const struct Move move, const struct Board *board) {
	return board_square_const(board, move.target)->color == board->active_color;
}

bool
board_consume_castling_rights_if_legal(struct Board *board, const struct Move move) {
	struct Coord king_coordinates = move.source;
	bool result = true;
	result &= move_triggers_check(move, board);
	king_coordinates.rank += 1;
	result &= move_triggers_check(move, board);
	result &= board_square(board, king_coordinates)->piece == PIECE_NONE;
	king_coordinates.rank += 1;
	result &= move_triggers_attack(move, king_coordinates, board);
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

const struct Board BOARD_STARTPOS = {
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
