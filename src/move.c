#include <stdlib.h>
#include <stdbool.h>
#include "move.h"

struct Move
move_new(const struct Coord source, const struct Coord target) {
	return (struct Move) {
		.source = source,
		.target = target,
		.dir = coord_dir(source, target),
		.promotion = PIECE_NONE,
	};
}

struct Move
str_to_move(const char *move) {
	size_t len = strlen(move);
	if (len != 4 && len != 5) {
		return MOVE_NONE;
	}
	struct Coord source = str_to_coord(move, 2);
	struct Coord target = str_to_coord(move + 2, 2);
	struct Piece promotion = (len == 5) ? str[4] : PIECE_QUEEN;
	return (struct Move) {
		.source = source,
		.target = target,
		.dir = coord_dir(source, target),
		.promotion = promotion,
	};
}

char *
move_to_str(const struct Move move) {
	char *str = malloc(5);
	if (!str) {
		return NULL;
	}
	str[0] = coord_to_str(move.source);
	str[2] = coord_to_str(move.target);
	str[5] = (move.promotion == PIECE_NONE) ? '\0' : move.promotion;
	return str;
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

const struct Move MOVE_NONE = {
	.source = COORD_NONE,
	.target = COORD_NONE,
	.dir = DIR_NONE,
	.promotion = PIECE_NONE,
};
