#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "move.h"
#include "coord.h"
#include "piece.h"
#include "board.h"
#include "utils.h"

struct Move
move_new(const struct Coord source, const struct Coord target) {
	return (struct Move) {
		.source = source,
		.target = target,
		.dir = coord_dir(source, target),
		.promotion = PIECE_NONE,
	};
}

char *
move_to_str(const struct Move move, char *str) {
	if (!str) {
		str = malloc(5);
		if (!str) {
			return NULL;
		}
	}
	coord_to_str(move.source, str);
	coord_to_str(move.target, str + 2);
	str[5] = (move.promotion == PIECE_NONE) ? '\0' : move.promotion;
	return str;
}

struct Move
str_to_move(const char *str) {
	size_t len = strlen(str);
	if (len != 4 && len != 5) {
		return MOVE_NONE;
	}
	struct Coord source = str_to_coord(str);
	struct Coord target = str_to_coord(str + 2);
	enum Piece promotion = (len == 5) ? str[4] : PIECE_QUEEN;
	return (struct Move) {
		.source = source,
		.target = target,
		.dir = coord_dir(source, target),
		.promotion = promotion,
	};
}

bool
move_is_capture(const struct Move move, const struct Board *board) {
	return board_square_const(board, move.target)->piece != PIECE_NONE;
}

bool
move_is_obstructed(const struct Move move, const struct Board *board) {
	Rank rank;
	Rank rank_limit;
	File file;
	File file_limit;
	switch (move.dir) {
		case DIR_HORIZONTAL:
			rank = move.source.file;
			file = MIN(move.source.file, move.target.file);
			file_limit = MAX(move.source.file, move.target.file);
			while ((++file), file < file_limit) {
				if (board->squares[rank][file].piece != PIECE_NONE) {
					return true;
				}
			}
			break;
		case DIR_VERTICAL:
			rank = MIN(move.source.rank, move.target.rank);
			file = move.source.file;
			rank_limit = MAX(move.source.rank, move.target.rank);
			while ((++rank), rank < rank_limit) {
				if (board->squares[rank][file].piece != PIECE_NONE) {
					return true;
				}
			}
			break;
		case DIR_DIAGONAL:
			rank = MIN(move.source.rank, move.target.rank);
			file = move.source.file;
			rank_limit = MAX(move.source.rank, move.target.rank);
			while ((++rank), (++file), rank < rank_limit) {
				if (board->squares[rank][file].piece != PIECE_NONE) {
					return true;
				}
			}
			break;
		default:
			break;
	}
	return false;
}

bool
move_is_promotion(const struct Move move, const struct Board *board) {
	return move.target.rank == color_promoting_rank(board->active_color);
}

bool
move_is_en_passant(const struct Move move, const struct Board *board) {
	return coord_eq(move.target, board->en_passant_target);
}

bool
move_is_legal(const struct Move move, const struct Board *board) {
	if (!(coord_is_valid(move.source) && coord_is_valid(move.target))) {
		return false;
	}
	if (coord_eq(move.source, move.target)) {
		return false;
	}
	if (board_square_const(board, move.source)->color != board->active_color) {
		return false;
	}
	if (move_is_self_harm(move, board)) {
		return false;
	}
	switch (board_square_const(board, move.source)->piece) {
		case PIECE_PAWN:
			if (abs(move.source.rank - move.target.rank) != 1) {
				return false;
			}
			switch (move.dir) {
				case DIR_DIAGONAL:
					return move_is_capture(move, board);
				case DIR_VERTICAL:
					return !move_is_capture(move, board);
				default:
					return false;
			}
		case PIECE_KNIGHT:
			return move.dir == DIR_L_SHAPED;
		case PIECE_BISHOP:
			return move.dir == DIR_DIAGONAL && !move_is_obstructed(move, board);
		case PIECE_ROOK:
			return (move.dir == DIR_HORIZONTAL ||
				    move.dir == DIR_VERTICAL) &&
		   		   !move_is_obstructed(move, board);
		case PIECE_QUEEN:
			return (move.dir == DIR_HORIZONTAL ||
				    move.dir == DIR_VERTICAL ||
				    move.dir == DIR_DIAGONAL) &&
				   !move_is_obstructed(move, board);
		case PIECE_KING:
			return abs(move.source.rank - move.target.rank) <= 1 &&
				   abs(move.source.file - move.target.file) <= 1;
		default:
			return false;
	}
}

bool
move_triggers_attack(const struct Move move, const struct Coord coord, const struct Board *board) {
	return false;
	// TODO
}

bool
move_triggers_check(const struct Move move, const struct Board *board) {
	return false;
	// TODO
}

const struct Move MOVE_NONE = {
	.source = { 42, 9 },
	.target = { 43, 9 },
	.dir = DIR_NONE,
	.promotion = PIECE_NONE,
};
