/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chess/fen.h"
#include "UI/cmd.h"
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

/*  64 + | pieces
 *   7 + | slashes
 *   1 + | space
 *   1 + | active color
 *   1 + | space
 *   4 + | castling rights
 *   1 + | space
 *   2 + | en-passant target square
 *   1 + | space
 *   6 + | half-moves clock
 *   1 + | space
 *   6 + | full-moves
 * -----
 *  95 plus some extra safety space. */

enum
{
	FEN_SIZE = 128
};

const char *const FEN_SEPARATORS = " _";

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
	snprintf(fen, 13, "%zu %zu", position->reversible_moves_count, position->moves_count);
	return fen_copy;
}

int
position_init_from_fen_as_cmd(struct Position *position, struct Cmd *cmd)
{
	assert(position);
	assert(cmd);
	*position = POSITION_EMPTY;
	char *fen = cmd_current(cmd);
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
	fen = cmd_current(cmd);
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
	fen = cmd_current(cmd);
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
	fen = cmd_current(cmd);
	if (*fen && *(fen + 1)) {
		File file = char_to_file(*fen);
		Rank rank = char_to_rank(*(fen + 1));
		position->is_en_passant_available = file != FILE_NONE && rank != RANK_NONE;
		position->en_passant_target = square_new(file, rank);
	}
	fen = cmd_current(cmd);
	position->reversible_moves_count = strtol(fen, NULL, 10);
	fen = cmd_current(cmd);
	position->moves_count = strtol(fen, NULL, 10);
	return 0;
}
