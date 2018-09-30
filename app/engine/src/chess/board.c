#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitboards.h"
#include "chess/board.h"
#include "chess/coord.h"
#include "chess/color.h"
#include "chess/move.h"
#include "utils.h"

#define BB_960_HELPER 0x100000000000001

void
board_setup_960(struct Board *board) {
	File available_files[8] = {0, 1, 2, 3, 4, 5, 6, 7};
	File file;
	file = available_files[(rand() % 4) * 2];
	board->bb_pieces[PIECE_BISHOP] |= BB_960_HELPER << file;
	available_files[file] = available_files[7];
	file = available_files[(rand() % 4) * 2 + 1];
	board->bb_pieces[PIECE_BISHOP] |= BB_960_HELPER << file;
	available_files[file] = available_files[6];
	file = available_files[rand() % 6];
	board->bb_pieces[PIECE_BISHOP] |= BB_960_HELPER << file;
	board->bb_pieces[PIECE_ROOK] |= BB_960_HELPER << file;
	available_files[file] = available_files[5];
	file = available_files[rand() % 5];
	board->bb_pieces[PIECE_KNIGHT] |= BB_960_HELPER << file;
	available_files[file] = available_files[4];
	file = available_files[rand() % 4];
	board->bb_pieces[PIECE_KNIGHT] |= BB_960_HELPER << file;
	available_files[file] = available_files[3];
	// Sort the remaining three elements.
	if (available_files[0] > available_files[2]) {
		available_files[7] = available_files[0];
		available_files[0] = available_files[2];
		available_files[2] = available_files[7];
	}
	if (available_files[0] > available_files[1]) {
		available_files[7] = available_files[0];
		available_files[0] = available_files[1];
		available_files[1] = available_files[7];
	}
	if (available_files[1] > available_files[2]) {
		available_files[7] = available_files[1];
		available_files[1] = available_files[2];
		available_files[2] = available_files[7];
	}
	board->bb_pieces[PIECE_ROOK] |= BB_960_HELPER << available_files[0];
	board->bb_pieces[PIECE_KING] |= BB_960_HELPER << available_files[1];
	board->bb_pieces[PIECE_ROOK] |= BB_960_HELPER << available_files[2];
}

enum Piece
board_piece_at(struct Board *board, Coord coord) {
	uint64_t bb = bb_coord(coord);
	if (board->bb_pieces[PIECE_PAWN] & bb) {
		return PIECE_PAWN;
	} else if (board->bb_pieces[PIECE_KNIGHT] & bb) {
		return PIECE_KNIGHT;
	} else if (board->bb_pieces[PIECE_BISHOP] &
			   board->bb_pieces[PIECE_ROOK] &
			   bb) {
		return PIECE_QUEEN;
	} else if (board->bb_pieces[PIECE_BISHOP] & bb) {
		return PIECE_BISHOP;
	} else if (board->bb_pieces[PIECE_ROOK] & bb) {
		return PIECE_ROOK;
	} else if (board->bb_pieces[PIECE_KING] & bb) {
		return PIECE_KING;
	} else {
		return PIECE_NONE;
	}
}

char
board_square_to_char(struct Board *board, Coord coord, char empty_square) {
	uint64_t bb = bb_coord(coord);
	if (board->bb_occupancy & bb) {
		enum Piece piece = board_piece_at(board, coord);
		if (board_color_at(board, coord) == COLOR_WHITE) {
			return toupper(piece_to_char(piece));
		} else {
			return tolower(piece_to_char(piece));
		}
	} else {
		return empty_square;
	}
}

enum Color
board_color_at(struct Board *board, Coord coord) {
	return board->bb_colors & bb_coord(coord) ? COLOR_BLACK : COLOR_WHITE;
}

void
board_print(struct Board *board) {
	File file;
	Rank rank = BOARD_SIDE_LENGTH;
	printf("    A B C D E F G H\n");
	printf("  ╔═════════════════╗\n");
	while (rank-- > 0) {
		printf("%c ║ ", rank_to_char(rank));
		for (file = 0; file < BOARD_SIDE_LENGTH; file++) {
			putchar(board_square_to_char(board, coord_new(file, rank), '.'));
			putchar(' ');
		}
		printf("║ %c\n", rank_to_char(rank));
	}
	printf("  ╚═════════════════╝\n");
	printf("    A B C D E F G H\n");
}

enum Color
board_active_color(struct Board *board) {
	return board->game_state >> GAME_STATE_ACTIVE_COLOR_OFFSET;
}

void
board_toggle_active_color(struct Board *board) {
	board->game_state ^= (1 << GAME_STATE_ACTIVE_COLOR_OFFSET);
}


size_t
board_num_half_moves(struct Board *board) {
	return (board->game_state & GAME_STATE_NUM_HALF_MOVES) >> 8;
}

void
board_increment_half_moves(struct Board *board) {
	// FIXME
	board->game_state &= (board_num_half_moves(board) + 1) << 8;
}

void
board_reset_half_moves(struct Board *board) {
	// FIXME
	board->game_state &= !GAME_STATE_NUM_HALF_MOVES;
}

int_fast8_t
board_castling_rights(struct Board *board) {
	return board->game_state >> 4 & 0xf;
}

bool
board_en_passant_is_available(struct Board *board) {
	return board->game_state & 0xff8f;
}

Coord
board_en_passant_coord(struct Board *board) {
	return bb_file(board->game_state & 0x7) &
		   bb_rank(color_en_passant_rank(board_active_color(board)));
}

size_t
board_legal_moves(struct Board *board, Move *move) {
	return 1337; // TODO
}

// A lot of shuffling around with bits and bytes is involved with this, but it's
// actually rather simple.
const struct Board BOARD_STARTPOS = {
	.bb_occupancy = 0xffff00000000ffff,
	.bb_colors = 0xffffffff00000000,
	.bb_pieces = {
		0x00ff00000000ff00,
		0x4200000000000042,
		0x2c0000000000002c,
		0x8900000000000089,
		0x1000000000000010,
	},
	.game_state = GAME_STATE_CASTLING_RIGHT_ALL,
};

const struct Board BOARD_NONE = {
	.bb_occupancy = 0x150d80c4a13cee73,
	.bb_colors = 0x05d8bdab3dc4614f,
	.bb_pieces = {
		0x19b0628a126d722e,
		0xdaf6c76749348c8e,
		0xdd117e1fd1cf0015,
		0xc09a9b009ce7d6c0,
		0xc9509da366a8e6a7,
	},
	.game_state = 0xf0,
};
