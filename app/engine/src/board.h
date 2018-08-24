#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "color.h"
#include "piece.h"

#define BOARD_SIDE_LENGTH 8
#define BOARD_NUM_SQUARES 64
#define BOARD_NUM_PIECES 32
#define BOARD_MAX_HALF_MOVES 75
#define BOARD_AVERAGE_NUM_CANDIDATE_MOVES 28

#define GAME_STATE_ACTIVE_COLOR 0x8000
#define GAME_STATE_ACTIVE_COLOR_OFFSET 31
#define GAME_STATE_NUM_HALF_MOVES 0x7f00
#define GAME_STATE_NUM_HALF_MOVES_OFFSET 8
#define GAME_STATE_CASTLING_RIGHT_ALL 0xf0
#define GAME_STATE_CASTLING_RIGHT_WK 0x80
#define GAME_STATE_CASTLING_RIGHT_WQ 0x40
#define GAME_STATE_CASTLING_RIGHT_BK 0x20
#define GAME_STATE_CASTLING_RIGHT_BQ 0x10
#define GAME_STATE_EN_PASSANT_AVAILABILITY 0x8
#define GAME_STATE_EN_PASSANT_FILE 0x7

#define BOARD_BB_PAWNS 0
#define BOARD_BB_ROOKS 1
#define BOARD_BB_KNIGHTS 2
#define BOARD_BB_BISHOPS 3
#define BOARD_BB_KINGS 4

// A piece-centric, space-efficient board representation that ships with
// relevant game state information. This solution is based on bitboards and
// lends itself to fast legality checking while mantaining a tiny memory
// footprint (the whole thing fits comfortably in 64 bytes).
struct Board {
	uint64_t bb_occupancy;
	uint64_t bb_colors;
	// A bitboard per chess piece type, excluding queens that are considered
	// both rooks and bishops at the same time. It amounts to 40 bytes in total.
	uint64_t bb_pieces[5];
	// Game state information:
	// * active color (1 bit),
	// * number of half moves (7 bits),
	// * castling rights (4 bits: WK, WQ, BK, BQ),
	// * en passant availability (1 bit), and
	// * en passant file (3 bits),
	// for a total of 2 bytes.
	uint16_t game_state;
};

// Print this chess position to stdout.
void
board_print(struct Board *board);

// Get the ASCII character that represents a certain square in this chess
// position.
char
board_square_to_char(struct Board *board, Coord coord);

// Get the color of the player whose turn it is.
enum Color
board_active_color(struct Board *board);

// Get the number of half moves since any capture, pawn, or castling move was
// made. Please note that this number can't possibly be any larger than 75,
// after which the game is automatically a draw.
uint_fast8_t
board_num_half_moves(struct Board *board);

// Get the castling right of a certain player on the side of 'rook'.
bool
board_castling_right(struct Board *board, uint_fast8_t bitmask);

// Get the coordinates to the square where en passant capture is possible, or
// 'COORD_NONE' if none.
Coord
board_en_passant_coord(struct Board *board);

extern const struct Board BOARD_STARTPOS;
