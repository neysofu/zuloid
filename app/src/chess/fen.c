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
 *   4 + | half-moves clock
 *   5 + | full-moves
 *   5 = | spaces
 * -----
 *  92 plus some extra safety space. */
enum
{
	FEN_SIZE = 127
};

const char *const FEN_SEPARATORS = " _";

char *
fen_new_from_position(const struct Position *position)
{
	assert(position);
	char *fen = malloc_or_exit(FEN_SIZE);
	char *fen_copy = fen;
	Piece pieces_by_square[64];
	position_list_pieces_by_square(position, pieces_by_square);
	for (Rank rank = RANK_MAX; rank >= 0; rank--) {
		size_t free_files_count = 0;
		for (File file = 0; file <= FILE_MAX; file++) {
			Square square = square_new(file, rank);
			Piece piece = pieces_by_square[square];
			if (PIECE_TYPE_NONE == piece) {
				if (free_files_count++) {
					*(fen - 1) += 1;
				} else {
					*fen++ = '1';
				}
			} else {
				*fen++ = piece_to_char(piece);
				free_files_count = 0;
			}
		}
		*fen++ = '/';
	}
	/* The last character was set to '/' in the loop but it must be a space. */
	*(fen - 1) = ' ';
	*fen++ = color_to_char(position->side_to_move);
	*fen++ = ' ';
	if (position->castling_rights) {
		if (position->castling_rights & CASTLING_RIGHT_W_OO) {
			*fen++ = 'K';
		}
		if (position->castling_rights & CASTLING_RIGHT_W_OOO) {
			*fen++ = 'Q';
		}
		if (position->castling_rights & CASTLING_RIGHT_B_OO) {
			*fen++ = 'k';
		}
		if (position->castling_rights & CASTLING_RIGHT_B_OOO) {
			*fen++ = 'q';
		}
	} else {
		*fen++ = '-';
	}
	*fen++ = ' ';
	if (position->is_en_passant_available) {
		*fen++ = file_to_char(square_file(position->en_passant_target));
		*fen++ = rank_to_char(square_rank(position->en_passant_target));
	} else {
		*fen++ = '-';
	}
	*fen++ = ' ';
	snprintf(fen, 12, "%d %d", position->reversible_moves_count, position->moves_count);
	return fen_copy;
}

bool
fen_go_to_next_token(const char **fen)
{
	*fen += strcspn(*fen, FEN_SEPARATORS);
	*fen += strspn(*fen, FEN_SEPARATORS);
	return **fen;
}

int
position_set_from_fen(struct Position *position, const char *fen)
{
	if (!fen) {
		return 0;
	}
	*position = POSITION_EMPTY;
	/* Ranks are marked by slashed, so we need fen++ to get past them. */
	for (Rank rank = RANK_MAX; rank >= 0; rank--) {
		for (File file = 0; *fen && file <= FILE_MAX; fen++, file++) {
			Square square = square_new(file, rank);
			if (isdigit(*fen)) {
				file += *fen - '1';
			} else {
				position_set_piece_at_square(position, square, char_to_piece(*fen));
			}
		}
		if ('/' == *fen) {
			fen++;
		}
	}
	fen_go_to_next_token(&fen);
	if (*fen) {
		switch (tolower(*fen)) {
			case 'w':
				break;
			case 'b':
				position->side_to_move = COLOR_BLACK;
				break;
			default:
				return -1;
		}
	}
	fen_go_to_next_token(&fen);
	while (isalpha(*fen)) {
		switch (*fen) {
			case 'K':
				position->castling_rights |= CASTLING_RIGHT_W_OO;
				break;
			case 'Q':
				position->castling_rights |= CASTLING_RIGHT_W_OOO;
				break;
			case 'k':
				position->castling_rights |= CASTLING_RIGHT_B_OO;
				break;
			case 'q':
				position->castling_rights |= CASTLING_RIGHT_B_OOO;
				break;
		}
		fen++;
	}
	fen_go_to_next_token(&fen);
	if (*fen && *(fen + 1)) {
		File file = char_to_file(*fen);
		Rank rank = char_to_rank(*(fen + 1));
		Square square = square_new(file, rank);
		position->is_en_passant_available = square != SQUARE_NONE;
		position->en_passant_target = square;
	}
	fen_go_to_next_token(&fen);
	position->reversible_moves_count = strtol(fen, NULL, 10);
	fen_go_to_next_token(&fen);
	position->moves_count = strtol(fen, NULL, 10);
	return 0;
}
