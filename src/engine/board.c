#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "coord.h"
#include "color.h"
#include "move.h"
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

void
board_print(const struct Board *board) {
	struct Coord coord = {
		.file = 0,
		.rank = BOARD_SIDE_LENGTH,
	};
	while (coord.rank-- > 0) {
		while (coord.file < BOARD_SIDE_LENGTH) {
			putchar(square_to_char(*board_square(board, coord)));
			putchar(' ');
			coord.file++;
		}
		coord.file = 0;
		putchar('\n');
	}
	putchar('\n');
}

struct Square *
board_square(const struct Board *board, const struct Coord coord) {
	return &(board->squares[coord.rank][coord.file]);
}

void
board_update_num_half_moves_and_num_pieces(struct Board *board, const struct Move move) {
	bool is_capture = move_is_capture(move, board);
	if (is_capture) {
		board->num_pieces--;
	}
	if (is_capture || board_square(board, move.source)->piece == PIECE_PAWN) {
		board->num_half_moves = 0;
	} else {
		board->num_half_moves++;
	}
}

/*
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
}*/

struct Square *
board_en_passant_square(const struct Board *board) {
	struct Coord coord = {
		.file = board->en_passant_file,
		.rank = color_en_passant_rank(board->active_color),
	};
	return board_square(board, coord);
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
		{ PIECE_NONE, COLOR_NONE },
		{ PIECE_NONE, COLOR_NONE },
		{ PIECE_NONE, COLOR_NONE },
		{ PIECE_NONE, COLOR_NONE },
		{ PIECE_NONE, COLOR_NONE },
		{ PIECE_NONE, COLOR_NONE },
		{ PIECE_NONE, COLOR_NONE },
		{ PIECE_NONE, COLOR_NONE },
		},
		{
		{ PIECE_NONE, COLOR_NONE },
		{ PIECE_NONE, COLOR_NONE },
		{ PIECE_NONE, COLOR_NONE },
		{ PIECE_NONE, COLOR_NONE },
		{ PIECE_NONE, COLOR_NONE },
		{ PIECE_NONE, COLOR_NONE },
		{ PIECE_NONE, COLOR_NONE },
		{ PIECE_NONE, COLOR_NONE },
		},
		{
		{ PIECE_NONE, COLOR_NONE },
		{ PIECE_NONE, COLOR_NONE },
		{ PIECE_NONE, COLOR_NONE },
		{ PIECE_NONE, COLOR_NONE },
		{ PIECE_NONE, COLOR_NONE },
		{ PIECE_NONE, COLOR_NONE },
		{ PIECE_NONE, COLOR_NONE },
		{ PIECE_NONE, COLOR_NONE },
		},
		{
		{ PIECE_NONE, COLOR_NONE },
		{ PIECE_NONE, COLOR_NONE },
		{ PIECE_NONE, COLOR_NONE },
		{ PIECE_NONE, COLOR_NONE },
		{ PIECE_NONE, COLOR_NONE },
		{ PIECE_NONE, COLOR_NONE },
		{ PIECE_NONE, COLOR_NONE },
		{ PIECE_NONE, COLOR_NONE },
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
	.castling_rights = 0b1111,
	.num_pieces = 16,
};
