#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "fen.h"
#include "board.h"
#include "move.h"
#include "coord.h"
#include "utils.h"

char *
board_to_fen(struct Board *board, char *fen) {
	if (!fen) {
		fen = malloc(FEN_MAX_LENGTH);
		if (!fen) {
			return NULL;
		}
	}
	char *fen_start = fen;
	Coord coord = coord_new(0, 7);
	while (coord_rank(coord) > 0) {
		while (coord_file(coord) < BOARD_SIDE_LENGTH - 1) {
			//if (board->bb_occupancy[coord]) {
			//	*fen = piece_to_char(board_piece_at(board, coord));
			//	if (board_square(board, coord)->color == COLOR_BLACK) {
			//		*fen = tolower(*fen);
			//	}
			//} else {
			//	*fen = '1';
			//}
			coord++;
		}
		coord -= BOARD_SIDE_LENGTH;
		*(fen++) = '/';
	}
	*(fen - 1) = ' ';
	*(fen++) = board_active_color(board) ? 'w' : 'b';
	*(fen++) = ' ';
	if (board_castling_right(board, GAME_STATE_CASTLING_RIGHT_WK)) {
		*(fen++) = 'K';
	}
	if (board_castling_right(board, GAME_STATE_CASTLING_RIGHT_BK)) {
		*(fen++) = 'k';
	}
	if (board_castling_right(board, GAME_STATE_CASTLING_RIGHT_WQ)) {
		*(fen++) = 'Q';
	}
	if (board_castling_right(board, GAME_STATE_CASTLING_RIGHT_BQ)) {
		*(fen++) = 'q';
	}
	if (*(fen - 1) == ' ') {
		*(fen++) = '-';
	}
	*(fen++) = ' ';
	if (coord_is_valid(board_en_passant_coord(board))) {
		coord_to_str(board_en_passant_coord(board), fen);
		fen += 2;
	} else {
		*(fen++) = '-';
	}
	*(fen++) = ' ';
	sprintf(fen, "%u 0", board_num_half_moves(board));
	fen += 4;
	*fen = '\0';
	return fen_start;
}

void
fen_to_board(char *fen, struct Board *board) {
	char *token;
	token = strtok_r(fen, WHITESPACE_CHARS, &token);
	uint8_t i = 0;
	while (token[i]) {
		switch (token[i++]) {
			case 'K':
				board->game_state |= GAME_STATE_CASTLING_RIGHT_WK;
				break;
			case 'k':
				board->game_state |= GAME_STATE_CASTLING_RIGHT_BK;
				break;
			case 'Q':
				board->game_state |= GAME_STATE_CASTLING_RIGHT_WQ;
				break;
			case 'q':
				board->game_state |= GAME_STATE_CASTLING_RIGHT_BQ;
				break;
			default:
				break;
		}
	}
	token = strtok_r(fen, WHITESPACE_CHARS, &token);
	if (token[0] != '-') {
		board->game_state &= !GAME_STATE_EN_PASSANT_AVAILABILITY;
	}
	token = strtok_r(fen, WHITESPACE_CHARS, &token);
	board->game_state |= GAME_STATE_NUM_HALF_MOVES & atoi(token);
}

void
fen_specify_pieces(char *fen_fragment, struct Board *board) {
	//char *token;
	//Coord coord = coord_new(0, BOARD_SIDE_LENGTH);
	//while ((token = strtok_r(fen_fragment, "/", &fen_fragment)), coord_rank(coord)-- > 0) {
	//	uint_fast8_t i = 0;
	//	while (coord.file < BOARD_SIDE_LENGTH && token[i]) {
	//		if (isdigit(token[i])) {
	//			while (token[i]-- >= '1' && coord_file(coord) < BOARD_SIDE_LENGTH) {
	//				//*board_piece_at(board, coord) = SQUARE_NONE; TODO
	//				coord.file++;
	//			}
	//		} else {
	//			// TODO: sanitize input.
	//			if (isupper(token[i])) {
	//				board_square(board, coord)->color = COLOR_WHITE;
	//				if (token[i] == 'K') {
	//					board->game_state[COLOR_WHITE].king = coord;
	//				}
	//			} else {
	//				token[i] = toupper(token[i]);
	//				board_square(board, coord)->color = COLOR_BLACK;
	//				if (token[i] == 'k') {
	//					board->color_info[COLOR_BLACK].king = coord;
	//				}
	//			}
	//			board_square(board, coord)->piece = token[i];
	//			coord.file++;
	//		}
	//		i++;
	//	}
	//	coord.file = 0;
	//}
}
