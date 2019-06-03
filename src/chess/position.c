/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chess/position.h"
#include "chess/color.h"
#include "chess/coordinates.h"
#include "chess/move.h"
#include "chess/pieces.h"
#include "utils.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum CastlingRights
char_to_castling_right(char c)
{
	switch (c) {
		case 'K':
			return CASTLING_RIGHT_WK;
		case 'Q':
			return CASTLING_RIGHT_WQ;
		case 'k':
			return CASTLING_RIGHT_BK;
		case 'q':
			return CASTLING_RIGHT_BQ;
		default:
			return CASTLING_RIGHT_NONE;
	}
}

enum CastlingRights
string_to_castling_rights(const char *str)
{
	assert(str);
	enum CastlingRights crights = CASTLING_RIGHT_NONE;
	for (; *str; str++) {
		crights |= char_to_castling_right(*str);
	}
	return crights;
}

void
position_init_960(struct Position *position)
{
	const Bitboard BB_A1_OR_A8 = 0x0100000000000001;
	position_empty(position);
	File available_files[FILES_COUNT] = { 0, 1, 2, 3, 4, 5, 6, 7 };
	int i = (rand() % 4) * 2;
	position->bb[PIECE_TYPE_BISHOP] |= BB_A1_OR_A8 << available_files[i];
	available_files[i] = available_files[7];
	i = (rand() % 4) * 2 + 1;
	position->bb[PIECE_TYPE_BISHOP] |= BB_A1_OR_A8 << available_files[i];
	available_files[i] = available_files[6];
	i = rand() % 6;
	position->bb[PIECE_TYPE_BISHOP] |= BB_A1_OR_A8 << available_files[i];
	position->bb[PIECE_TYPE_ROOK] |= BB_A1_OR_A8 << available_files[i];
	available_files[i] = available_files[5];
	i = rand() % 5;
	position->bb[PIECE_TYPE_KNIGHT] |= BB_A1_OR_A8 << available_files[i];
	available_files[i] = available_files[4];
	i = rand() % 4;
	position->bb[PIECE_TYPE_KNIGHT] |= BB_A1_OR_A8 << available_files[i];
	available_files[i] = available_files[3];
	if (available_files[0] > available_files[1]) {
		available_files[7] = available_files[0];
		available_files[0] = available_files[1];
		available_files[1] = available_files[7];
	}
	if (available_files[1] > available_files[2]) {
		available_files[7] = available_files[1];
		available_files[1] = available_files[2];
		available_files[2] = available_files[7];
	}
	if (available_files[0] > available_files[1]) {
		available_files[7] = available_files[0];
		available_files[0] = available_files[1];
		available_files[1] = available_files[7];
	}
	position->bb[PIECE_TYPE_ROOK] |= BB_A1_OR_A8 << available_files[0];
	position->bb[PIECE_TYPE_ROOK] |= BB_A1_OR_A8 << available_files[1];
	position->bb[PIECE_TYPE_ROOK] |= BB_A1_OR_A8 << available_files[2];
}

void
position_set_piece_at_square(struct Position *position, Square square, struct Piece piece)
{
	Bitboard bb = square_to_bb(square);
	for (size_t i = 0; i < POSITION_BB_COUNT; i++) {
		position->bb[i] &= ~bb;
	}
	switch (piece.type) {
		case PIECE_TYPE_QUEEN:
			position->bb[PIECE_TYPE_BISHOP] |= bb;
			position->bb[PIECE_TYPE_ROOK] |= bb;
			break;
		case PIECE_TYPE_NONE:
			return;
		default:
			position->bb[piece.type] |= bb;
			break;
	}
	position->bb[piece.color] |= bb;
}

struct Piece
position_piece_at_square(const struct Position *position, Square square)
{
	assert(position);
	Bitboard bb = square_to_bb(square);
	struct Piece piece = PIECE_NONE;
	enum PieceType i = PIECE_TYPE_FIRST_PRIMITIVE;
	do {
		if (position->bb[i] & bb) {
			piece.type += i;
		}
	} while (i++ < PIECE_TYPE_LAST_PRIMITIVE);
	if (position->bb[COLOR_BLACK] & bb) {
		piece.color = COLOR_BLACK;
	}
	return piece;
}

void
position_flip_side_to_move(struct Position *position)
{
	position->side_to_move ^= 1;
}

void
position_empty(struct Position *position)
{
	assert(position);
	*position = (struct Position){
		.side_to_move = COLOR_WHITE,
		.en_passant_target = SQUARE_NONE,
		.castling_rights = CASTLING_RIGHTS_ALL,
		.reversible_moves_count = 0,
		.moves_count = 1,
	};
}

const struct Position POSITION_INIT = {
	.bb =
	  {
	    [COLOR_WHITE] = 0x0303030303030303,
	    [COLOR_BLACK] = 0xc0c0c0c0c0c0c0c0,
	    [PIECE_TYPE_PAWN] = 0x4242424242424242,
	    [PIECE_TYPE_KNIGHT] = 0x0081000000008100,
	    [PIECE_TYPE_BISHOP] = 0x0000810081810000,
	    [PIECE_TYPE_ROOK] = 0x8100000081000081,
	    [PIECE_TYPE_KING] = 0x0000008100000000,
	  },
	.side_to_move = COLOR_WHITE,
	.en_passant_target = SQUARE_NONE,
	.castling_rights = CASTLING_RIGHTS_ALL,
	.reversible_moves_count = 0,
	.moves_count = 1,
};
