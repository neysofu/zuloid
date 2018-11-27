/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * A piece centric board representation. */

#pragma once

#include "chess/color.h"
#include "chess/piece.h"
#include <stdbool.h>
#include <stdint.h>

#define BOARD_SIDE_LENGTH 8
#define BOARD_NUM_SQUARES 64
#define BOARD_NUM_PIECES 32
#define BOARD_MAX_HALF_MOVES 75
#define BOARD_AVERAGE_NUM_CANDIDATE_MOVES 28
#define BOARD_MAX_NUM_LEGAL_MOVES 230

/// A mask for the active color.
#define GAME_STATE_ACTIVE_COLOR_OFFSET 15
#define GAME_STATE_NUM_HALF_MOVES 0x7f00
#define GAME_STATE_NUM_HALF_MOVES_OFFSET 8
#define GAME_STATE_CASTLING_RIGHT_ALL 0xf0
#define GAME_STATE_CASTLING_RIGHT_WK 0x80
#define GAME_STATE_CASTLING_RIGHT_WQ 0x40
#define GAME_STATE_CASTLING_RIGHT_BK 0x20
#define GAME_STATE_CASTLING_RIGHT_BQ 0x10
#define GAME_STATE_EN_PASSANT_AVAILABILITY 0x8
#define GAME_STATE_EN_PASSANT_FILE 0x7

/// @}

// A piece-centric, space-efficient board representation that ships with
// relevant game state information. This solution is based on bitboards and
// lends itself to fast legality checking while mantaining a tiny memory
// footprint (the whole thing fits comfortably in 64 bytes).
struct Board
{
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

/// @see
/// https://en.wikipedia.org/wiki/Chess960_numbering_scheme#Direct_derivation
void
board_setup_960(struct Board *board, int16_t seed);

uint64_t
board_hash(struct Board *board);

enum Piece
board_piece_at(struct Board *board, Coord coord);

enum Color
board_color_at(struct Board *board, Coord coord);

// Returns the lowercase letter that FEN uses to represent the piece at 'coord';
// if none, returns ".".
char
board_square_to_char(struct Board *board, Coord coord, char empty_square);

// Returns the color of the player whose turn it is.
enum Color
board_active_color(struct Board *board);

void
board_toggle_active_color(struct Board *board);

// Returns the number of half moves since any capture, pawn, or castling movewas
// was made. Please note that this number can't possibly be any larger than 75,
// after which the game is automatically a draw.
size_t
board_num_half_moves(struct Board *board);

void
board_increment_half_moves(struct Board *board);

void
board_reset_half_moves(struct Board *board);

// Get the castling right of a certain player on the side of 'rook'.
int_fast8_t
board_castling_rights(struct Board *board);

// Checks whether or not the current player can capture en-passant.
bool
board_en_passant_is_available(struct Board *board);

// Returns the coordinates of the square where en passant capture is possible,
// or 'COORD_NONE' if none.
Coord
board_en_passant_coord(struct Board *board);

extern const struct Board BOARD_STARTPOS;
extern const struct Board BOARD_NONE;
