#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fen.h"
#include "board.h"
#include "move.h"
#include "coord.h"
#include "utils.h"

char *
board_to_fen(const struct Board *board, char *fen) {
	if (!fen) {
		fen = malloc(FEN_MAX_LENGTH);
		if (!fen) {
			return NULL;
		}
	}
	Rank rank;
	File file;
	for (rank = 0; rank < BOARD_SIDE_LENGTH; rank++) {
		for (file = 0; file < BOARD_SIDE_LENGTH; file++) {
			*fen = board->squares[rank][file].piece;
			if (board->squares[rank][file].piece != PIECE_NONE) {
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
	if (board->castling_rights & CASTLING_RIGHT_WHITE_KINGSIDE) {
		*(fen++) = 'K';
	}
	if (board->castling_rights & CASTLING_RIGHT_BLACK_KINGSIDE) {
		*(fen++) = 'k';
	}
	if (board->castling_rights & CASTLING_RIGHT_WHITE_QUEENSIDE) {
		*(fen++) = 'Q';
	}
	if (board->castling_rights & CASTLING_RIGHT_BLACK_QUEENSIDE) {
		*(fen++) = 'q';
	}
	if (*(fen - 1) == ' ') {
		*(fen++) = '-';
	}
	*(fen++) = ' ';
	if (!file_is_valid(board->en_passant_file)) {
		*(fen++) = '-';
	} else {
		*(fen++) = ' '; // FIXME
		*(fen++) = file_to_char(board->en_passant_file);
	}
	*(fen++) = ' ';
	snprintf(fen++, 2, "%d", board->num_half_moves);
	*(++fen) = '\0';
	return fen - FEN_MAX_LENGTH;
}

struct Board *
fen_to_board(char *fen, struct Board *board) {
	char *fen_iter_ptr = NULL;
	if (!board) {
		board = board_new(NULL);
		if (!board) {
			return NULL;
		}
	}
	fen_specify_pieces(fen, board);
	if (!fen) {
		return NULL;
	}
	fen = strtok_r(NULL, " ", &fen_iter_ptr);
	if (!fen) {
		return NULL;
	}
	fen_specify_active_color(fen, board);
	fen = strtok_r(NULL, " ", &fen_iter_ptr);
	fen_specify_castling_rights(fen, board);
	if (!fen) {
		return NULL;
	}
	fen = strtok_r(NULL, " ", &fen_iter_ptr);
	fen_specify_en_passant_file(fen, board);
	if (!fen) { return NULL; }
	fen = strtok_r(NULL, " ", &fen_iter_ptr);
	if (!fen) { return NULL; }
	fen_specify_half_moves(fen, board);
	fen = strtok_r(NULL, " ", &fen_iter_ptr);
	if (!fen) { return NULL; }
	return board;
}

void
fen_specify_pieces(char *fen_fragment, struct Board *board) {
	/*Rank rank = BOARD_SIDE_LENGTH;
	File file = 0;
	char *token;
	char *token_iter_ptr;
	struct Coord coord = COORD_A8;
	while ((token = strtok_r(fen_fragment, "/", &token_iter_ptr)), rank-->0) {
		// N.B.: 'atoi' returns 0 when the string is not a valid number.
		while (file < BOARD_SIDE_LENGTH) {
			coord.file += MAX(atoi(token + file), 1);
			if (token[token_index])
			if (token[file] > 'a') {
				board_square(board, coord)->color = COLOR_BLACK;
				token[file] += ('A' - 'a');
			} else {
				board_square(board, .color)->color = COLOR_WHITE;
			}
			board->squares[rank][file].piece = token[file];
		}
	}*/
}

void
fen_specify_active_color(char *fen_fragment, struct Board *board) {
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
fen_specify_castling_rights(char *fen_fragment, struct Board *board) {
	uint8_t i = 0;
	while (fen_fragment[i]) {
		switch (fen_fragment[i++]) {
			case 'K':
				board->castling_rights |= CASTLING_RIGHT_WHITE_KINGSIDE;
				break;
			case 'k':
				board->castling_rights |= CASTLING_RIGHT_BLACK_KINGSIDE;
				break;
			case 'Q':
				board->castling_rights |= CASTLING_RIGHT_WHITE_QUEENSIDE;
				break;
			case 'q':
				board->castling_rights |= CASTLING_RIGHT_BLACK_QUEENSIDE;
				break;
			case '-':
				board->castling_rights |= CASTLING_RIGHT_WHITE_KINGSIDE;
				board->castling_rights |= CASTLING_RIGHT_BLACK_KINGSIDE;
				board->castling_rights |= CASTLING_RIGHT_WHITE_QUEENSIDE;
				board->castling_rights |= CASTLING_RIGHT_BLACK_QUEENSIDE;
				break;
			default:
				break;
		}
	}
}

void
fen_specify_en_passant_file(char *fen_fragment, struct Board *board) {
	if (fen_fragment[0] == '-') {
		board->en_passant_file = FILE_NONE;
	} else {
		board->en_passant_file = str_to_coord(fen_fragment).file;
	}
}

void
fen_specify_half_moves(char *fen_fragment, struct Board *board) {
	board->num_half_moves = atoi(fen_fragment);
}

void
fen_specify_full_moves(char *fen_fragment, struct Board *board) {
	// TODO
}
