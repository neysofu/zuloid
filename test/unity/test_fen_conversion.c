/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "Unity/src/unity.h"
#include "chess/fen.h"
#include "chess/position.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *const VALID_FEN[] = {
	"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
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
	"R2K1B1R/PPP2QPP/5N1N/1n2q3/8/1p3n2/pbpp3p/1kr4r w KQkq - 0 1",
	"8/3p4/1P2R1p1/7p/pP5R/1p4r1/2p3p1/k1K2B2 w - - 0 0",
	"1KB2R1R/2P1B3/1P1Q4/1n3N2/qpp2n1P/8/6pp/r3rbk1 w - - 0 0",
	"2kr4/K1pp4/1p6/8/8/8/7Q/3R4 w - - 2 23",
	"r6r/ppp2Bpp/2n1b3/2R5/3k4/B7/P1P1KPPP/8 w - - 0 13",
	"r2q1rk1/pb1nbppp/1pp1pn2/3pN3/2PPP3/2N3P1/PP3PBP/R1BQ1RK1 w - - 0 1",
	"4R1K1/P5P1/3P1PP1/3BR3/5b2/p1p2p2/1Q3qpp/r3rk2 b - - 0 1",
	"1KR4R/PPPB1P1P/4Q1Pq/4P1b1/1B1Nn3/3p3p/ppp2pp1/1kr2bnr w - - 0 1",
	/* "rnbqkbnr/pppppppp/8/9/3P4/8/PPP1PPPP/RNBQKBNR b AHah d4 0 1",
	 * "rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR b aAhH d4 0 1",
	 * "bnrqkr1b/pppppppp/5n2/8/4P3/8/PPPP1PPP/BNRQKRNB w cFfC - 1 2", */
	"",
};

void
test_fen_conversion(void)
{
	for (size_t i = 0; *VALID_FEN[i]; i++) {
		struct Position position;
		char fen[FEN_SIZE];
		strcpy(fen, VALID_FEN[i]);
		position_init_from_fen(&position, fen);
		char *f = fen_from_position(fen, &position, ' ');
		TEST_ASSERT_EQUAL_STRING(VALID_FEN[i], f);
	}
}
