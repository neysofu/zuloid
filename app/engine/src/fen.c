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

// See [0] for reference.
//
//  64 + | pieces
//   7 + | slashes
//   1 + | active color
//   4 + | castling rights
//   2 + | en-passant target square
//   2 + | half-moves clock
//   4 + | full-moves
//   5 = | spaces
// -----
//  89
//
// [0]: https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
#define FEN_MAX_LENGTH 89

char *
board_to_fen(struct Board *board, char *fen) {
	if (!fen) {
		char *fen = malloc(FEN_MAX_LENGTH);
		if (!fen) {
			return NULL;
		}
	}
	char *fen_start = fen;
	Rank rank = BOARD_SIDE_LENGTH;
	File file;
	while (rank-- > 0) {
		for (file = 0; file < BOARD_SIDE_LENGTH; file++) {
			Coord coord = coord_new(file, rank);
			uint64_t bb_coord = coord_to_bb(coord);
			if (board->bb_occupancy & bb_coord) {
				*fen = piece_to_char(board_piece_at(board, coord));
				if (board->bb_colors & bb_coord) {
					*fen = tolower(*fen);
				}
			} else {
				*fen = '1';
			}
			coord++;
		}
		*(fen++) = '/';
	}
	*(fen - 1) = ' ';
	*(fen++) = board_active_color(board) ? 'w' : 'b';
	*(fen++) = ' ';
	if (board_castling_rights(board) & GAME_STATE_CASTLING_RIGHT_WK) {
		*(fen++) = 'K';
	}
	if (board_castling_rights(board) & GAME_STATE_CASTLING_RIGHT_BK) {
		*(fen++) = 'k';
	}
	if (board_castling_rights(board) & GAME_STATE_CASTLING_RIGHT_WQ) {
		*(fen++) = 'Q';
	}
	if (board_castling_rights(board) & GAME_STATE_CASTLING_RIGHT_BQ) {
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

struct Board *
fen_to_board(char *fen, struct Board *board) {
	//char *token;
	//char *save_ptr;
	//token = strtok_r(fen, WHITESPACE_CHARS, &token);
	//Coord coord = coord_new(0, BOARD_SIDE_LENGTH);
	//while ((token = strtok_r(fen, "/", &save_ptr)), coord_rank(coord--) > 0) {
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
	//// Active color
	//token = strtok_r(NULL, WHITESPACE_CHARS, &save_ptr);
	//if (!token) {
	//	return;
	//}
	//switch (tolower(token[0])) {
	//	case 'w':
	//		board->game_state |= COLOR_WHITE << GAME_STATE_ACTIVE_COLOR_OFFSET;
	//		break;
	//	case 'b':
	//		board->game_state |= COLOR_BLACK << GAME_STATE_ACTIVE_COLOR_OFFSET;
	//		break;
	//	default:
	//		break;
	//}
	//// Castling rights
	//token = strtok_r(NULL, WHITESPACE_CHARS, &save_ptr);
	//if (!token) {
	//	return;
	//}
	//size_t i = 0;
	//while (token[i]) {
	//	switch (token[i++]) {
	//		case 'K':
	//			board->game_state |= GAME_STATE_CASTLING_RIGHT_WK;
	//			break;
	//		case 'k':
	//			board->game_state |= GAME_STATE_CASTLING_RIGHT_BK;
	//			break;
	//		case 'Q':
	//			board->game_state |= GAME_STATE_CASTLING_RIGHT_WQ;
	//			break;
	//		case 'q':
	//			board->game_state |= GAME_STATE_CASTLING_RIGHT_BQ;
	//			break;
	//		case '-':
	//			// TODO
	//			board->game_state;
	//			break;
	//		default:
	//			goto en_passant;
	//	}
	//}
	//// En passant
//en_passant:
	//token = strtok_r(NULL, WHITESPACE_CHARS, &save_ptr);
	//if (!token) {
	//	return;
	//} else if (token[0] != '-') {
	//	board->game_state &= !GAME_STATE_EN_PASSANT_AVAILABILITY;
	//}
	//// Num. halfmoves
	//token = strtok_r(NULL, WHITESPACE_CHARS, &save_ptr);
	//if (!token) {
	//	return;
	//}
	//board->game_state |= GAME_STATE_NUM_HALF_MOVES & atoi(token);
	//// Num. fullmoves
	//token = strtok_r(NULL, WHITESPACE_CHARS, &save_ptr);
	//if (!token) {
	//	return;
	//}
	//board->game_state |= GAME_STATE_NUM_HALF_MOVES & atoi(token);
	return board;
}
