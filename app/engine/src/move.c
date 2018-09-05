#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "bitboards.h"
#include "move.h"
#include "coord.h"
#include "piece.h"
#include "board.h"
#include "result.h"
#include "utils.h"

typedef Move uint_fast32_t;

Move
move_new(Coord source, Coord target, enum Piece promotion) {
	return (promotion << 24) +
		   (coord_dir(source, target) << 16) +
		   (source << 8) +
		   target;
}

Coord
move_source(Move move) {
	return (move >> 8) & 0xff;
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
	return move_new(source, target, promotion);
}

const Move MOVE_NONE = UINT32_MAX;
