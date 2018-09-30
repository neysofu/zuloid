#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "bitboards.h"
#include "chess/board.h"
#include "chess/fen.h"
#include "chess/move.h"
#include "chess/coord.h"
#include "utils.h"

void
board_to_fen(struct Board *board, char *buffer) {
	assert(board);
	assert(buffer);
	Rank rank = BOARD_SIDE_LENGTH;
	File file;
	while (rank-- > 0) {
		for (file = 0; file < BOARD_SIDE_LENGTH; file++) {
			Coord coord = coord_new(file, rank);
			uint64_t bb = bb_coord(coord);
			*buffer++ = board_square_to_char(board, coord, '1');
		}
		*(buffer++) = '/';
	}
	*(buffer - 1) = ' ';
	*(buffer++) = board_active_color(board) ? 'w' : 'b';
	*(buffer++) = ' ';
	if (board_castling_rights(board) & GAME_STATE_CASTLING_RIGHT_WK) {
		*(buffer++) = 'K';
	}
	if (board_castling_rights(board) & GAME_STATE_CASTLING_RIGHT_BK) {
		*(buffer++) = 'k';
	}
	if (board_castling_rights(board) & GAME_STATE_CASTLING_RIGHT_WQ) {
		*(buffer++) = 'Q';
	}
	if (board_castling_rights(board) & GAME_STATE_CASTLING_RIGHT_BQ) {
		*(buffer++) = 'q';
	}
	if (*(buffer - 1) == ' ') {
		*(buffer++) = '-';
	}
	*(buffer++) = ' ';
	if (board_en_passant_coord(board) == COORD_NONE) {
		*(buffer++) = '-';
	} else {
		Coord en_passant_coord = board_en_passant_coord(board);
		*(buffer++) = file_to_char(coord_file(en_passant_coord));
		*(buffer++) = rank_to_char(coord_rank(en_passant_coord));
	}
	*(buffer++) = ' ';
	sprintf(buffer, "%zu 0", board_num_half_moves(board));
	buffer += 4;
	*buffer = '\0';
}

int8_t
fen_to_board(char *fen, struct Board *buffer) {
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
	return 0;
}
