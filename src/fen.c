#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fen.h"
#include "board.h"
#include "coord.h"
#include "utils.h"

// See [1] for reference.
//
//  64 + | pieces
//   7 + | slashes
//   1 + | active color
//   4 + | castling rights
//   2 + | en-passant target square
//   2 + | half-moves clock
//   3 + | full-moves
//   5 = | spaces
// -----
//  88
//
// [1]: https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
#define FEN_LENGTH 88

struct Board *
fen_to_board(char *fen) {
	struct Chessboard *board = malloc(sizeof(struct Chessboard));
	*board = BOARD_INIT;
	char *token;
	if (!(token = strtok(fen, " "))) {
		return NULL;
	}
	board_specify_pieces(board, token);
	if (!(token = strtok(fen, " "))) {
		return NULL;
	}
	board_specify_active_player(board, token);
	if (!(token = strtok(fen, " "))) {
		return NULL;
	}
	board_specify_castling_rights(board, token);
	if (!(token = strtok(fen, " "))) {
		return NULL;
	}
	board_specify_en_passant_target(board, token);
	if (!(token = strtok(fen, " "))) {
		return NULL;
	}
	board_specify_half_moves(board, token);
	return board;
}

char *
board_to_fen(struct Board *board) {
	char *fen = malloc(FEN_LENGTH);
	char *fen_start = fen;
	Rank rank = 0;
	File file = 0;
	for (; rank < BOARD_SIDE_LENGTH; rank++) {
		for (; file < BOARD_SIDE_LENGTH; file++) {
			*fen = board->squares[rank][file].piece;
			if (board->squares[rank][file].color == *fen != PIECE_NONE) {
				*fen += ('a' - 'A');
			}
			fen++;
		}
		*(fen++) = '/';
	}
	*(fen - 1) = ' ';
	switch (board->active_color) {
		case COLOR_WHITE:
			*(fen++) = 'w';
			break;
		case COLOR_BLACK:
			*(fen++) = 'b';
			break;
		default:
			return NULL;
	}
	*(fen++) = ' ';
	if (board->castling_rights.white_kingside) {
		*(fen++) = 'K';
	}
	if (board->castling_rights.white_kingside) {
		*(fen++) = 'Q';
	}
	if (board->castling_rights.black_kingside) {
		*(fen++) = 'k';
	}
	if (board->castling_rights.black_kingside) {
		*(fen++) = 'q';
	}
	if (*(fen - 1) == ' ') {
		*(fen++) = '-';
	}
	*(fen++) = ' ';
	if (coord_eq(board->en_passant_target, COORD_NONE)) {
		*(fen++) = '-';
	} else {
		*(fen++) = rank_to_algebraic_notation(board->en_passant_target.rank);
		*(fen++) = file_to_algebraic_notation(board->en_passant_target.file);
	}
	*(fen++) = ' ';
	snprintf(fen++, 2, "%d", board->half_moves);
	*(++fen) = '\0';
	return fen_start;
}

void
board_specify_pieces(struct Board *board, char *fen_fragment) {
	Rank rank = CHESSBOARD_SIDE_LENGHT;
	File file = 0;
	char *token;
	while ((token = strtok(fen_fragment, "/")), rank-->0) {
		// N.B.: 'atoi' returns 0 when the string is not a valid number.
		file += MAX(atoi(token + file), 1);
		while (file < CHESSBOARD_SIDE_LENGHT) {
			if (token[file] > 'a') {
				board->squares[rank][file].color = COLOR_BLACK;
				token[file] += ('A' - 'a');
			} else {
				board->squares[rank][file].color = COLOR_WHITE;
			}
			board->squares[rank][file].piece = token[file];
		}
	}
}

void
board_specify_active_player(struct Board *board, char *fen_fragment) {
	switch (fen_fragment[0]) {
		case 'w':
			board->active_color = COLOR_WHITE;
			break;
		case 'b':
			board->active_color = COLOR_BLACK;
			break;
		default:
			return;
	}
}

void
board_specify_castling_rights(struct Board *board, char *fen_fragment) {
	uint8_t i = 0;
	while (fen_fragment[i] != '\0') {
		switch (fen_fragment[i]) {
			case 'K':
				board->castling_rights.white_kingside = true;
				break;
			case 'Q':
				board->castling_rights.white_queenside = true;
				break;
			case 'k':
				board->castling_rights.black_kingside = true;
				break;
			case 'q':
				board->castling_rights.black_queenside = true;
				break;
			case '-':
				board->castling_rights.white_kingside = true;
				board->castling_rights.white_queenside = true;
				board->castling_rights.black_kingside = true;
				board->castling_rights.black_queenside = true;
				break;
			default:
				return;
		}
		i++;
	}
}

void
board_specify_en_passant_target(struct Board *board, char *fen_fragment) {
	if (fen_fragment[0] == '-') {
		board->en_passant_target = COORD_NONE;
	}
	board->en_passant_target = mv_from_algebraic_notation(fen_fragment).target;
}

void
board_specify_half_moves(struct Board *board, char *fen_fragment) {
	board->half_moves = atoi(fen_fragment);
}
