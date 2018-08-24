#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "move.h"
#include "coord.h"
#include "piece.h"
#include "board.h"
#include "result.h"
#include "utils.h"

typedef Move uint_fast32_t;

Move
move_new(Coord source, Coord target, enum Piece promotion) {
	return promotion << 24 +
		   coord_dir(source, target) << 16 +
		   source << 8 +
		   target;
}

Coord
move_source(Move move) {
	return (move >> 8) && 0xff;
}

Coord
move_target(Move move) {
	return move & 0xff;
}

enum Piece
move_promotion(Move move) {
	return move >> 24;
}

enum Dir
move_dir(Move move) {
	return (move >> 16) & 0xff;
}

char *
move_to_str(Move move, char *str) {
	if (!str) {
		str = malloc(6);
		if (!str) {
			return NULL;
		}
	}
	coord_to_str(move_source(move), str);
	coord_to_str(move_target(move), str + 2);
	if (move_promotion(move) == PIECE_NONE) {
		str[4] = '\0';
	} else {
		str[4] = piece_to_char(move_promotion(move));
		str[5] = '\0';
	}
	return str;
}

Move
str_to_move( char *str) {
	size_t len = strlen(str);
	if (len < 4) {
		return MOVE_NONE;
	}
	Coord source = str_to_coord(str);
	Coord target = str_to_coord(str + 2);
	bool is_promotion = len >= 5 && isalpha(str[4]);
	enum Piece promotion = is_promotion ? toupper(str[4]) : PIECE_QUEEN;
	return (Move) {
		.source = source,
		.target = target,
		.dir = coord_dir(source, target),
		.promotion = promotion,
	};
}

bool
move_is_obstructed( Move move,  struct Board *board) {
	Rank rank;
	Rank rank_limit;
	File file;
	File file_limit;
	switch (move.dir) {
		case DIR_HORIZONTAL:
			rank = move.source.rank;
			file = MIN(move.source.file, move.target.file);
			file_limit = MAX(move.source.file, move.target.file);
			while (++file < file_limit) {
				if (board->squares[rank][file].piece != PIECE_NONE) {
					return true;
				}
			}
			break;
		case DIR_VERTICAL:
			file = move.source.file;
			rank = MIN(move.source.rank, move.target.rank);
			rank_limit = MAX(move.source.rank, move.target.rank);
			while (++rank < rank_limit) {
				if (board->squares[rank][file].piece != PIECE_NONE) {
					return true;
				}
			}
			break;
		case DIR_DIAGONAL:
			rank = MIN(move.source.rank, move.target.rank);
			rank_limit = MAX(move.source.rank, move.target.rank);
			if ((move.source.rank > move.target.rank) ^
				(move.source.file > move.target.file)
			) {
				// * . . . .
				// . * . . .
				// . . * . .
				// . . . * .
				// . . . . *
				file = MAX(move.source.file, move.target.file);
				while (--file, ++rank < rank_limit) {
					if (board->squares[rank][file].piece != PIECE_NONE) {
						return true;
					}
				}
			} else {
				// . . . . *
				// . . . * .
				// . . * . .
				// . * . . .
				// * . . . .
				file = MIN(move.source.file, move.target.file);
				while (++file, ++rank < rank_limit) {
					if (board->squares[rank][file].piece != PIECE_NONE) {
						return true;
					}
				}
			}
			break;
		default:
			break;
	}
	return false;
}

bool
move_is_capture(Move move, struct Board *board) {
	return board->bb_occupancy[move_target(move)];
}

bool
move_is_promotion(Move move, struct Board *board) {
	return coord_rank(move_target(move)) ==
		   color_promoting_rank(board_active_color(board));
}

bool
move_is_en_passant(Move move,  struct Board *board) {
	return move.target.file == board->en_passant_file;
}

bool
move_is_self_harm(Move move, struct Board *board) {
	return board->bb_colors[move_target(move)];
}

bool
move_is_legal_by_pawn(Move move, struct Board *board) {
	if (move.source.rank < move.target.rank == board->active_color) {
		return false;
	}
	return move->bb_pawns[]
	Rank home_rank = color_pawn_rank(board->active_color);
	bool is_on_home_rank = move.source.rank == home_rank;
	switch (move.dir) {
		case DIR_DIAGONAL:
			return move_is_capture(move, board) &&
				   abs(move.source.rank - move.target.rank) == 1;
		case DIR_VERTICAL:
			return !move_is_capture(move, board) &&
				   !move_is_obstructed(move, board) &&
				   abs(move.source.rank - move.target.rank) <= 1 + is_on_home_rank;
		default:
			return false;
	}
}

bool
move_is_legal_by_king(Move move, struct Board *board) {
	if (abs(move.source.rank - move.target.rank) <= 1 &&
		abs(move.source.file - move.target.file) <= 1) {
		return true;
	}
	if (!(move.dir == DIR_HORIZONTAL &&
		  move.source.rank == color_home_rank(board->active_color))) {
		return false;
	}
	switch (move.source.file - move.target.file) {
		case 2:
			return board->color_info[board->active_color].castling_rights & CASTLING_RIGHT_KINGSIDE;
		case -3:
			return board->color_info[board->active_color].castling_rights & CASTLING_RIGHT_QUEENSIDE;
		default:
			return false;
	}
}

bool
move_is_legal(Move move, Piece piece
	if (move_source(move) == move_target(move) ||
	    (board->bb_colors & coord_to_bb(coord) == board_active_color(board)) ||
	    move_is_self_harm(move, board)) {
		return false;
	}
	switch (board_square(board, move.source)->piece) {
		case PIECE_PAWN:
			return move_is_legal_by_pawn(move, board);
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
			return move_is_legal_by_king(move, board);
		default:
			return false;
	}
}

bool
move_is_castling(Move move, struct Board *board) {
	return board_square(board, move.source)->piece == PIECE_KING &&
		   abs(move.source.file - move.target.file) == 2;
}

bool
move_triggers_attack( Move move, Coord coord,  struct Board *board) {
	return false;
	// TODO
}

bool
move_triggers_check( Move move,  struct Board *board) {
	return false;
}

struct Result
board_push(struct Board *board, Move move) {
	board_update_num_half_moves(board, move);
	struct Square *source = board_square(board, move.source);
	struct Square *target = board_square(board, move.target);
	if (target->piece == PIECE_KING) {
		return (struct Result) {
			.winner = board->active_color,
			.termination = TERMINATION_CHECKMATE,
		};
	}
	// Deal with special moves: en passant captures, castling, and promotion.
	if (move_is_en_passant(move, board)) {
		board_en_passant_square(board)->piece = PIECE_NONE;
	} else if (move_is_promotion(move, board)) {
		if (!move.promotion) {
			move.promotion = PIECE_QUEEN;
		}
		target->piece = move.promotion;
		board->color_info[board->active_color].mating_material += piece_mating_material(move.promotion);
	} else if (move_is_castling(move, board)) {
		File rook_source_file;
		File rook_target_file;
		if (move.target.file == 2) {
			rook_source_file = 0;
			rook_target_file = 3;
		} else {
			rook_source_file = 7;
			rook_target_file = 5;
		}
		Coord rook = coord_new(rook_source_file, move.source.rank);
		*board_square(board, rook) = SQUARE_NONE;
		rook.file = rook_target_file;
		board_square(board, rook)->piece = PIECE_ROOK;
		board_square(board, rook)->color = board->active_color;
	}
	// Now the board is actually updated.
	board->color_info[board->active_color].mating_material -= piece_mating_material(target->piece);
	*target = *source;
	*source = SQUARE_NONE;
	board->active_color = color_other(board->active_color);
	// Gameover detection. TODO: finish this.
	if (board->num_half_moves >= MAX_HALF_MOVES) {
		return (struct Result) {
			.winner = COLOR_NONE,
			.termination = TERMINATION_NO_CAPTURE,
		};
	}
	return RESULT_NONE;
}

void
board_update_num_half_moves(struct Board *board,  Move move) {
	bool is_capture = move_is_capture(move, board);
	if (is_capture || board_square(board, move.source)->piece == PIECE_PAWN) {
		board->num_half_moves = 0;
	} else {
		board->num_half_moves++;
	}
}

 Move MOVE_NONE = {
	.dir = DIR_NONE,
	.promotion = PIECE_NONE,
};
