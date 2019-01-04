/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chess/fen.h"
#include "chess/castling.h"
#include "chess/color.h"
#include "chess/coordinates.h"
#include "chess/move.h"
#include "chess/piece_types.h"
#include "chess/position.h"
#include "utils.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*  64 + | pieces
 *   7 + | slashes
 *   1 + | active color
 *   4 + | castling rights
 *   2 + | en-passant target square
 *   2 + | half-moves clock
 *   4 + | full-moves
 *   5 = | spaces
 * -----
 *  89 plus some extra safety space. */
#define FEN_SIZE (92)

const char *const FEN_SEPARATORS = " _";

char *
fen_new_from_position(const struct Position *position)
{
	assert(position);
	char *fen = malloc_or_exit(FEN_SIZE);
	uint_fast8_t free_files_count = 0;
	Piece pieces_by_square[64];
	position_list_pieces_by_square(position, pieces_by_square);
	for (Rank rank = 0; rank < RANK_MAX; rank++) {
		size_t free_files_count = 0;
		for (File file = 0; file < FILE_MAX; file++) {
			Piece piece = pieces_by_square[square_new(file, rank)];
			if (PIECE_TYPE_NONE == piece) {
				free_files_count++;
			} else if (free_files_count > 0) {
				*fen++ = free_files_count + '0';
			}
			*fen++ = piece_to_char(piece);
		}
		*fen++ = '/';
	}
	logg("Pieces are now listed in the FEN");
	/* The last character was set to '/' in the loop but it must be a space. */
	*(fen - 1) = ' ';
	*fen++ = color_to_char(position->side_to_move);
	*fen++ = ' ';
	logg("Color done");
	if (position->castling_rights & CASTLING_RIGHT_W_OO) {
		*fen++ = 'K';
	}
	if (position->castling_rights & CASTLING_RIGHT_B_OO) {
		*fen++ = 'k';
	}
	if (position->castling_rights & CASTLING_RIGHT_W_OOO) {
		*fen++ = 'Q';
	}
	if (position->castling_rights & CASTLING_RIGHT_B_OOO) {
		*fen++ = 'q';
	}
	if (*(fen - 1) == ' ') {
		*fen++ = '-';
	}
	*fen++ = ' ';
	if (position->en_passant_target != SQUARE_NONE) {
		*fen++ = file_to_char(square_file(position->en_passant_target));
		*fen++ = rank_to_char(square_rank(position->en_passant_target));
	} else {
		*fen++ = '-';
	}
	*fen++ = ' ';
	// TODO: fullmove number.
	sprintf(fen, "%d 1", position->reversible_moves_count);
	fen += 4;
	*fen = '\0';
	return fen;
}

int
position_set_from_fen(struct Position *position, const char *original_fen)
{
	char fen[FEN_SIZE];
	logg("Setting pieces up");
	strcpy(fen, original_fen);
	char *save_ptr;
	char *token = strtok_r(fen, FEN_SEPARATORS, &save_ptr);
	if (!token) {
		return -1;
	}
	Rank rank = RANK_MAX;
	File file = 0;
	uint_fast8_t free_files_count = 0;
	for (size_t i = 0; token[i] && rank + 1 && file + 1; i++) {
		Square square = square_new(file, rank);
		if (token[i] == '/') {
			rank--;
			file = 0;
		} else if (isdigit(token[i])) {
			while (token[i]-- > '0') {
				position_set_piece_at_square(position, square, PIECE_TYPE_NONE);
			}
		} else {
			position_set_piece_at_square(position, square, char_to_piece(token[i]));
		}
	}
	logg("Pieces are set");
	if (!(token = strtok_r(fen, FEN_SEPARATORS, &save_ptr))) {
		return -1;
	}
	switch (tolower(*token)) {
		case 'w':
			break;
		case 'b':
			position->side_to_move = COLOR_BLACK;
			break;
		default:
			return -1;
	}
	logg("Correctly parsed the side to move");
	if (!(token = strtok_r(fen, FEN_SEPARATORS, &save_ptr))) {
		return -1;
	}
	for (size_t i = 0; token[i]; i++) {
		switch (token[i]) {
			case 'K':
				position->castling_rights |= CASTLING_RIGHT_W_OO;
				break;
			case 'k':
				position->castling_rights |= CASTLING_RIGHT_B_OO;
				break;
			case 'Q':
				position->castling_rights |= CASTLING_RIGHT_W_OOO;
				break;
			case 'q':
				position->castling_rights |= CASTLING_RIGHT_B_OOO;
				break;
			case '-':
				token[i++] = '\0';
				break;
			default:
				return -1;
		}
	}
	token = strtok_r(NULL, FEN_SEPARATORS, &save_ptr);
	if (!token) {
		return -1;
	}
	token = strtok_r(NULL, FEN_SEPARATORS, &save_ptr);
	if (!token) {
		return -1;
	}
	position->reversible_moves_count = strtol(fen, NULL, 10);
	logg("Side to move: %d", position->side_to_move);
	return 0;
}
