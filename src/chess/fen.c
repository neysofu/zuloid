/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chess/fen.h"
#include "chess/color.h"
#include "chess/coordinates.h"
#include "chess/move.h"
#include "chess/pieces.h"
#include "chess/position.h"
#include "utils.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *
fen_write_position_pieces(char *fen, const struct Position *position)
{
	Rank rank = RANK_MAX;
	do {
		bool last_piece_type_was_none = false;
		for (File file = 0; file <= FILE_MAX; file++) {
			Square square = square_new(file, rank);
			struct Piece piece = position_piece_at_square(position, square);
			if (piece.type) {
				*fen++ = piece_to_char(piece);
				last_piece_type_was_none = false;
			} else if (last_piece_type_was_none) {
				*(fen - 1) += 1;
			} else {
				*fen++ = '1';
				last_piece_type_was_none = true;
			}
		}
		if (rank) {
			*fen++ = '/';
		}
	} while (rank--);
	return fen;
}

char *
fen_new_from_position(const struct Position *position)
{
	assert(position);
	char *fen = malloc(FEN_SIZE);
	if (!fen) {
		return NULL;
	}
	char *fen_copy = fen;
	fen = fen_write_position_pieces(fen, position);
	*fen++ = ' ';
	*fen++ = color_to_char(position->side_to_move);
	*fen++ = ' ';
	if (position->castling_rights) {
		if (position->castling_rights & CASTLING_RIGHT_WK) {
			*fen++ = 'K';
		}
		if (position->castling_rights & CASTLING_RIGHT_WQ) {
			*fen++ = 'Q';
		}
		if (position->castling_rights & CASTLING_RIGHT_BK) {
			*fen++ = 'k';
		}
		if (position->castling_rights & CASTLING_RIGHT_BQ) {
			*fen++ = 'q';
		}
	} else {
		*fen++ = '-';
	}
	*fen++ = ' ';
	if (position->en_passant_target == SQUARE_NONE) {
		*fen++ = '-';
	} else {
		*fen++ = file_to_char(square_file(position->en_passant_target));
		*fen++ = rank_to_char(square_rank(position->en_passant_target));
	}
	*fen++ = ' ';
	snprintf(fen, 13, "%zu %zu", position->reversible_moves_count, position->moves_count);
	return fen_copy;
}

int
position_init_from_fen(struct Position *position, char *fen)
{
	assert(position);
	assert(fen);
	*position = POSITION_EMPTY;
	char *token = strsep_whitespace(fen);
	/* Ranks are marked by slashed, so we need fen++ to get past them. */
	for (Rank rank = RANK_MAX; rank >= 0; rank--) {
		for (File file = 0; *token && file <= FILE_MAX; token++, file++) {
			Square square = square_new(file, rank);
			if (isdigit(*token)) {
				file += *token - '1';
			} else {
				position_set_piece_at_square(position, square, char_to_piece(*token));
			}
		}
		if ('/' == *token) {
			token++;
		}
	}
	token = strsep_whitespace(fen);
	switch (tolower(*token)) {
		case 'w':
			position->side_to_move = COLOR_WHITE;
			break;
		case 'b':
			position->side_to_move = COLOR_BLACK;
			break;
		default:
			return ERR_CODE_INVALID_FEN;
	}
	token = strsep_whitespace(fen);
	while (*token) {
		position->castling_rights |= char_to_castling_right(*token++);
	}
	token = strsep_whitespace(fen);
	if (strlen(token) >= 2) {
		File file = char_to_file(token[0]);
		Rank rank = char_to_rank(token[1]);
		position->en_passant_target = square_new(file, rank);
	}
	token = strsep_whitespace(fen);
	position->reversible_moves_count = strtol(token, NULL, 10);
	token = strsep_whitespace(fen);
	position->moves_count = strtol(token, NULL, 10);
	return ERR_CODE_NONE;
}
