#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "coord.h"
#include "color.h"
#include "move.h"
#include "utils.h"

enum Piece
board_piece_at(struct Board *board, Coord coord) {
	uint64_t bb_coord = coord_to_bb(coord);
	if (board->bb_pieces[PIECE_PAWN] & bb_coord) {
		return PIECE_PAWN;
	} else if (board->bb_pieces[PIECE_KNIGHT] & bb_coord) {
		return PIECE_KNIGHT;
	} else if (board->bb_pieces[PIECE_BISHOP] &
			   board->bb_pieces[PIECE_ROOK] &
			   bb_coord) {
		return PIECE_QUEEN;
	} else if (board->bb_pieces[PIECE_BISHOP] &
			   bb_coord) {
		return PIECE_BISHOP;
	} else if (board->bb_pieces[PIECE_ROOK] & bb_coord) {
		return PIECE_ROOK;
	} else if (board->bb_pieces[PIECE_KING] & bb_coord) {
		return PIECE_KING;
	} else {
		return PIECE_NONE;
	}
}

char
board_square_to_char(struct Board *board, Coord coord) {
	uint64_t bb_coord = coord_to_bb(coord);
	if (board->bb_occupancy & bb_coord) {
		return piece_to_char(board_piece_at(board, coord));
	} else {
		return '.';
	}
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
			putchar(board_square_to_char(board, coord_new(file, rank)));
			putchar(' ');
		}
		printf("║ %c\n", rank_to_char(rank));
	}
	printf("  ╚═════════════════╝\n");
	printf("    A B C D E F G H\n");
	printf("\nIt's %s's turn.\n", board_active_color(board) ? "black" : "white");
	if (board->game_state & GAME_STATE_CASTLING_RIGHT_WK) {
		printf("White can castle kingside.\n");
	}
	if (board->game_state & GAME_STATE_CASTLING_RIGHT_WQ) {
		printf("White can castle queenside.\n");
	}
	if (board->game_state & GAME_STATE_CASTLING_RIGHT_BK) {
		printf("Black can castle kingside.\n");
	}
	if (board->game_state & GAME_STATE_CASTLING_RIGHT_BQ) {
		printf("Black can castle queenside.\n");
	}
}

enum Color
board_active_color(struct Board *board) {
	return board->game_state >> 31;
}

uint_fast8_t
board_num_half_moves(struct Board *board) {
	return (board->game_state & GAME_STATE_NUM_HALF_MOVES) >> 8;
}

bool
board_castling_right(struct Board *board, uint_fast8_t bitmask) {
	return board->game_state >> 4 & bitmask;
}

uint64_t
board_en_passant_bb(struct Board *board) {
	return bb_file(board->game_state & 0x7) &
		   bb_rank(color_en_passant_rank(board_active_color(board)));
}

// A lot of shuffling around with bits and bytes is involved with this, but it's
// actually rather simple.
const struct Board BOARD_STARTPOS = {
	.bb_occupancy = 0xffff00000000ffff,
	.bb_colors = 0xffffffff00000000,
	.bb_pieces = {
		0x00ff00000000ff00,
		0x9100000000000091,
		0x4200000000000042,
		0x3400000000000034,
		0x0800000000000008,
	},
	.game_state = GAME_STATE_CASTLING_RIGHT_ALL << 4,
};
