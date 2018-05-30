#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "xxhash.h"
#include "chessboard.h"
#include "coordinates.h"
#include "color.h"
#include "move.h"
#include "castling.h"
#include "vector.h"
#include "square.h"
#include "util.h"

const struct Square *
cb_square_const(const struct Chessboard *cb, const struct Coordinates coord) {
	return &(cb->squares[coord.rank][coord.file]);
}

struct Square *
cb_square(struct Chessboard *cb, const struct Coordinates coord) {
	return &(cb->squares[coord.rank][coord.file]);
}

bool
cb_is_move_capture(const struct Chessboard *cb, const struct Move mv) {
	return cb_square_const(cb, mv.to)->piece != PIECE_NONE;
}

void
cb_update_half_moves_and_num_pieces(struct Chessboard *cb, const struct Move mv) {
	bool is_capture = cb_is_move_capture(cb, mv);
	if (is_capture) {
		cb->num_pieces--;
	}
	if (is_capture || cb_square(cb, mv.from)->piece == PIECE_PAWN) {
		cb->half_moves = 0;
	} else {
		cb->half_moves++;
	}
}

bool
cb_is_threefold_repetition(const struct Chessboard *cb) {
	if (cb->half_moves < 6) {
		return false;
	}
	uint64_t current_hash = cb->history_by_hash[cb->half_moves];
	int i;
	for (i=cb->half_moves; i>=0; i-=2) {
		if (cb->history_by_hash[i] == current_hash) {
			return true;
		}
	}
	return false;
}

uint64_t
cb_hash(const struct Chessboard *cb) {
	size_t buffer_lenght = sizeof(struct Square) * CHESSBOARD_SIDE_LENGHT * CHESSBOARD_SIDE_LENGHT +
	    				   sizeof(struct CastlingRights) +
						   sizeof(uint8_t) +
						   sizeof(enum Color);
	return XXH64(cb, buffer_lenght, 0);
}

void
cb_update_history(struct Chessboard *cb) {
	cb->history_by_hash[cb->half_moves] = cb_hash(cb);
}

void
cb_move(struct Chessboard *cb, const struct Move mv) {
	struct Square *from = cb_square(cb, mv.from);
	struct Square *to = cb_square(cb, mv.to);
	from->piece = PIECE_NONE;
	to = from;
	if (cb_is_move_en_passant(cb, mv)) {
		cb_square(cb, *(cb->en_passant_target))->piece = PIECE_NONE;
	} else if (cb_is_move_promotion(cb, mv)) {
		cb_promote_pawn(cb, mv);
	}
}

bool
cb_is_move_en_passant(const struct Chessboard *cb, const struct Move mv) {
	return coord_eq(mv.to, *(cb->en_passant_target));
}

bool
cb_is_move_promotion(const struct Chessboard *cb, const struct Move mv) {
	return mv.to.rank == color_promoting_rank(cb->active_color);
}

bool
cb_is_stalemate(const struct Chessboard *cb, const struct Move mv) {
	return false; // TODO
}

bool
cb_is_check(const struct Chessboard *cb, const struct Move mv) {
	struct Move king_capture = mv_init(mv.to, cb->white.king);
	if (cb_is_move_legal(cb, king_capture)) {
		return true;
	} else {
		return cb_discovers_attack(cb, mv, cb->white.king);
	}
}


void
cb_update_en_passant_target(struct Chessboard *cb, struct Move mv) {
	cb->en_passant_target = &(mv.to);
}

void
cb_remove_en_passant_target(struct Chessboard *cb) {
	cb->en_passant_target = NULL;
}

bool
cb_is_move_pseudolegal_pawn(const struct Chessboard *cb, const struct Move mv) {
	if (mv_rank_change(mv) != 1) {
		return false;
	}
	bool is_capture = cb_is_move_capture(cb, mv);
	switch (mv.direction) {
		case DIR_DIAGONAL:
			return is_capture;
		case DIR_VERTICAL:
			return !is_capture;
		default:
			return false;
	}
}

bool
cb_is_move_pseudolegal_knight(const struct Chessboard *cb, const struct Move mv) {
	return mv.direction == DIR_L_SHAPED;
}

bool
cb_is_move_pseudolegal_bishop(const struct Chessboard *cb, const struct Move mv) {
	return mv.direction == DIR_DIAGONAL && !mv_is_obstructed(cb, mv);
}

bool
cb_is_move_pseudolegal_rook(const struct Chessboard *cb, const struct Move mv) {
	return (mv.direction == DIR_HORIZONTAL ||
		    mv.direction == DIR_VERTICAL) &&
		   !mv_is_obstructed(cb, mv);
}

bool
cb_is_move_pseudolegal_queen(const struct Chessboard *cb, const struct Move mv) {
	return (mv.direction == DIR_HORIZONTAL ||
		    mv.direction == DIR_VERTICAL ||
		    mv.direction == DIR_DIAGONAL) &&
		   !mv_is_obstructed(cb, mv);
}

bool
cb_is_move_pseudolegal_king(const struct Chessboard *cb, const struct Move mv) {
	return mv_rank_change(mv) <= 1 && mv_file_change(mv) <= 1;
}

bool
cb_is_move_capture_of_king(const struct Chessboard *cb, const struct Move mv) {
	return cb_square_const(cb, mv.to)->piece == PIECE_KING;
}

bool
cb_is_move_self_harm(const struct Chessboard *cb, const struct Move mv) {
	return cb_square_const(cb, mv.to)->color == cb->active_color;
}

bool
mv_is_obstructed(const struct Chessboard *cb, const struct Move mv) {
	Rank rank;
	Rank rank_limit;
	File file;
	File file_limit;
	switch (mv.direction) {
		case DIR_HORIZONTAL:
			rank = mv.from.file;
			file = MIN(mv.from.file, mv.to.file);
			file_limit = MAX(mv.from.file, mv.to.file);
			while ((++file), file < file_limit) {
				if (cb->squares[rank][file].piece != PIECE_NONE) {
					return true;
				}
			}
			break;
		case DIR_VERTICAL:
			rank = MIN(mv.from.rank, mv.to.rank);
			file = mv.from.file;
			rank_limit = MAX(mv.from.rank, mv.to.rank);
			while ((++rank), rank < rank_limit) {
				if (cb->squares[rank][file].piece != PIECE_NONE) {
					return true;
				}
			}
			break;
		case DIR_DIAGONAL:
			rank = MIN(mv.from.rank, mv.to.rank);
			file = mv.from.file;
			rank_limit = MAX(mv.from.rank, mv.to.rank);
			while ((++rank), (++file), rank < rank_limit) {
				if (cb->squares[rank][file].piece != PIECE_NONE) {
					return true;
				}
			}
			break;
		default:
			break;
	}
	return false;
}

void
cb_promote_pawn(struct Chessboard *cb, const struct Move mv) {
	cb_square(cb, mv.to)->piece = mv.promotion;
}

bool
cb_is_square_threatened(const struct Chessboard *cb, const struct Coordinates coord) {
	return false; // TODO
}

bool
cb_consume_castling_rights_if_legal(struct Chessboard *cb, const struct Move mv) {
	struct Coordinates king_coordinates = mv.from;
	bool result = true;
	result &= cb_is_square_threatened(cb, king_coordinates);
	king_coordinates.rank += 1;
	result &= cb_is_square_threatened(cb, king_coordinates);
	result &= cb_square(cb, king_coordinates)->piece == PIECE_NONE;
	king_coordinates.rank += 1;
	result &= cb_is_square_threatened(cb, king_coordinates);
	if (!result) {
		return result;
	}
	switch (cb_square(cb, mv.from)->color) {
		case COLOR_WHITE:
			if (mv.from.rank > mv.to.rank) {
				result = cb->castling_rights.white_queenside;
				cb->castling_rights.white_queenside = false;
			} else {
				result = cb->castling_rights.white_queenside;
				cb->castling_rights.white_kingside = false;
			}
		case COLOR_BLACK:
			if (mv.from.rank > mv.to.rank) {
				result = cb->castling_rights.black_queenside;
				cb->castling_rights.black_queenside = false;
			} else {
				result = cb->castling_rights.black_queenside;
				cb->castling_rights.black_kingside = false;
			}
		default:
			return false;
	}
	return result;
}

bool
cb_is_move_legal(const struct Chessboard *cb, const struct Move mv) {
	struct Square from;
	if (!(coord_exists(mv.from) && coord_exists(mv.to))) {
		return false;
	}
	if (coord_eq(mv.from, mv.to)) {
		return false;
	}
	if (from.color != cb->active_color) {
		return false;
	}
	if (cb_is_move_self_harm(cb, mv)) {
		return false;
	}
	switch (from.piece) {
		case PIECE_NONE:
			return false;
		case PIECE_PAWN:
			return cb_is_move_pseudolegal_pawn(cb, mv);
		case PIECE_KNIGHT:
			return cb_is_move_pseudolegal_knight(cb, mv);
		case PIECE_BISHOP:
			return cb_is_move_pseudolegal_bishop(cb, mv);
		case PIECE_ROOK:
			return cb_is_move_pseudolegal_rook(cb, mv);
		case PIECE_QUEEN:
			return cb_is_move_pseudolegal_queen(cb, mv);
		case PIECE_KING:
			return cb_is_move_pseudolegal_king(cb, mv);
	}
}

const struct Chessboard *cb_INIT = &(const struct Chessboard) {
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
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
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
	.castling_rights = { true, true, true, true },
};
