#include <stdbool.h>
#include <stdlib.h>
#include "chessboard.h"
#include "piece.h"
#include "color.h"

struct Chessboard *
cb_960_rand() {
	struct Chessboard *cb;
	File available_files[CHESSBOARD_SIDE_LENGHT] = { 0, 1, 2, 3, 4, 5, 6, 7 };
	// Bishops (on squares of opposing colors)
	File bishop_1 = rand() % 8;
	available_files[bishop_1] = 7;
	File bishop_2 = (rand() % 7) + 1 + (bishop_1 % 2);
	available_files[bishop_2] = 6;
	// Queen
	File queen = available_files[rand() % 6];
	available_files[queen] = 5;
	// Knights
	File knight_1 = available_files[rand() % 5];
	available_files[knight_1] = 4;
	File knight_2 = available_files[rand() % 4];
	available_files[knight_2] = 3;
	// King (between the rooks)
	File king = available_files[1];
	// Rooks
	File rook_1 = available_files[0];
	File rook_2 = available_files[2];
	// Populating the home ranks
	cb->squares[0][rook_1].piece = PIECE_ROOK;
	cb->squares[0][knight_1].piece = PIECE_KNIGHT;
	cb->squares[0][bishop_1].piece = PIECE_BISHOP;
	cb->squares[0][queen].piece = PIECE_QUEEN;
	cb->squares[0][king].piece = PIECE_KING;
	cb->squares[0][bishop_2].piece = PIECE_BISHOP;
	cb->squares[0][knight_2].piece = PIECE_KNIGHT;
	cb->squares[0][rook_2].piece = PIECE_ROOK;
	cb->squares[7][rook_1].piece = PIECE_ROOK;
	cb->squares[7][knight_1].piece = PIECE_KNIGHT;
	cb->squares[7][bishop_1].piece = PIECE_BISHOP;
	cb->squares[7][queen].piece = PIECE_QUEEN;
	cb->squares[7][king].piece = PIECE_KING;
	cb->squares[7][bishop_2].piece = PIECE_BISHOP;
	cb->squares[7][knight_2].piece = PIECE_KNIGHT;
	cb->squares[7][rook_2].piece = PIECE_ROOK;
	return cb;
}
