/* Copyright (c) Filippo Costa - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * PROPRIETARY AND CONFIDENTIAL */

#include "Unity/src/unity.h"
#include "chess/fen.h"
#include "chess/position.h"

void
test_castling_mask(void)
{
	struct Board pos = POSITION_INIT;
	TEST_ASSERT_EQUAL_INT64(0x1010100000000,
	                        position_castle_mask(&pos, CASTLING_RIGHT_KINGSIDE));
	TEST_ASSERT_EQUAL_INT64(0x101010000,
	                        position_castle_mask(&pos, CASTLING_RIGHT_QUEENSIDE));
	char fen[FEN_SIZE] = "rbbqnkrn/pppppppp/8/8/8/8/PPPPPPPP/RBBQNKRN b KQkq - 0 1";
	position_init_from_fen(&pos, fen);
	TEST_ASSERT_EQUAL_INT64(0x80800000000000,
	                        position_castle_mask(&pos, CASTLING_RIGHT_KINGSIDE));
	TEST_ASSERT_EQUAL_INT64(0x808080800000,
	                        position_castle_mask(&pos, CASTLING_RIGHT_QUEENSIDE));
}
