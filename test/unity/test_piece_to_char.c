/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "Unity/src/unity.h"
#include "chess/pieces.h"

void
test_piece_to_char(void)
{
	struct Piece piece = { .type = PIECE_TYPE_QUEEN, .color = COLOR_WHITE };
	TEST_ASSERT_EQUAL_INT('Q', piece_to_char(piece));
	piece = PIECE_NONE;
	TEST_ASSERT_EQUAL_INT('.', piece_to_char(piece));
	piece = (struct Piece){ .type = PIECE_TYPE_ROOK, .color = COLOR_BLACK };
	TEST_ASSERT_EQUAL_INT('r', piece_to_char(piece));
	piece = (struct Piece){ .type = PIECE_TYPE_PAWN, .color = COLOR_WHITE };
	TEST_ASSERT_EQUAL_INT('P', piece_to_char(piece));
}
