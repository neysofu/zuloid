/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chess/fen.h"
#include "Unity/src/unity.h"
#include "chess/position.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *const VALID_FEN[] = {
	"1r2r1k1/p4ppp/1pn5/3Q4/8/5N1P/PP3PP1/R5K1 b - - 0 2",
	"r3qbk1/pbp3pp/8/2p5/1rNnPP2/1PQPB3/2P3PP/R4R1K b - - 1 16",
	"4r1k1/1bq1rp1p/p2p2p1/1p2b3/3BP3/P1P5/1P3QPP/1B2RR1K w - - 0 26",
	"r4bk1/pp1b3r/3p1q2/P2Pp1n1/1P2PnP1/2N2B2/3B1NP1/2RQ1RK1 b - - 0 35",
	"r4rk1/1p3p2/3p3p/1PqPb3/p1P2pb1/3B1Pp1/P1Q3PP/2RNR2K b - - 30 42",
	"r4rk1/1p3p2/3p3p/1PqPb3/p1P2pb1/3B1Pp1/P1Q3PP/2RNR2K b - - 72 0",
	"rnbqk1nr/ppppppbp/6p1/8/2PP4/2N5/PP2PPPP/R1BQKBNR w KQkq - 1 1",
	"bnrqkrnb/pppppppp/8/8/8/6P1/PPPPPP1P/BNRQKRNB b KQkq - 0 1",
	"2rq2k1/1p1b1rpp/5b2/pN3p2/Q1Nnp3/P2P2P1/1P2PPBP/2R1R1K1 w - - 1 20",
	"rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR b KQkq d3 0 1",
	"rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR b KQkq d4 0 1",
	//"rnbqkbnr/pppppppp/8/9/3P4/8/PPP1PPPP/RNBQKBNR b AHah d4 0 1",
	//"rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR b aAhH d4 0 1",
	//"bnrqkr1b/pppppppp/5n2/8/4P3/8/PPPP1PPP/BNRQKRNB w cFfC - 1 2",
	"",
};

void
test_fen(void)
{
	for (size_t i = 0; VALID_FEN[i][0]; i++) {
		char *fen;
		struct Position position;
		position_set_from_fen(&position, VALID_FEN[i]);
		fen = fen_new_from_position(&position);
		TEST_ASSERT_EQUAL_STRING(VALID_FEN[i], fen);
		free(fen);
	}
}

void
test_fen_init(void)
{
	char *fen = fen_new_from_position(&POSITION_DEFAULT);
	TEST_ASSERT_EQUAL_STRING("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", fen);
	free(fen);
}
