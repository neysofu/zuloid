#include <cstdio>
#include <cstdlib>
#include <string>
#include "catch.hpp"
extern "C" {
#include "fen.h"
#include "board.h"
}

char fen[4][FEN_MAX_LENGTH + 1] = {
	"r3qbk1/pbp3pp/8/2p5/1rNnPP2/1PQPB3/2P3PP/R4R1K b - - 1 16",
	"4r1k1/1bq1rp1p/p2p2p1/1p2b3/3BP3/P1P5/1P3QPP/1B2RR1K w - - 0 26",
	"r4bk1/pp1b3r/3p1q2/P2Pp1n1/1P2PnP1/2N2B2/3B1NP1/2RQ1RK1 b - - 0 35",
	"r4rk1/1p3p2/3p3p/1PqPb3/p1P2pb1/3B1Pp1/P1Q3PP/2RNR2K b - - 3 42",
};

TEST_CASE("'fen_to_board' after 'board_to_fen' is idempotent.",
		  "[FEN][Board]") {
	struct Board *board_1 = board_new(NULL);
	struct Board *board_2 = board_new(NULL);
	char *fen_buffer = NULL;
	for (uint8_t i = 0; i < 4; i++) {
		INFO("The starting FEN is '" << fen[i] << "'.");
		board_1 = fen_to_board(fen[i], board_1);
		fen_buffer = board_to_fen(board_1, fen_buffer);
		board_2 = fen_to_board(fen_buffer, board_2);
		CHECK(memcmp(board_2, board_2, sizeof(struct Board)) == 0);
	}
	board_drop(board_1);
	board_drop(board_2);
	free(fen_buffer);
}
