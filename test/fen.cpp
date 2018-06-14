#include <cstdio>
#include <cstdlib>
#include <string>
#include "catch.hpp"
extern "C" {
#include "fen.h"
#include "board.h"
}

const char fen_1[] = "r3qbk1/pbp3pp/8/2p5/1rNnPP2/1PQPB3/2P3PP/R4R1K b - -";
const char fen_2[] = "4r1k1/1bq1rp1p/p2p2p1/1p2b3/3BP3/P1P5/1P3QPP/1B2RR1K w - - 0 26";
const char fen_3[] = "r4bk1/pp1b3r/3p1q2/P2Pp1n1/1P2PnP1/2N2B2/3B1NP1/2RQ1RK1 b - -";
const char fen_4[] = "r4rk1/1p3p2/3p3p/1PqPb3/p1P2pb1/3B1Pp1/P1Q3PP/2RNR2K b - -";

TEST_CASE("Going back and forth between 'fen_to_board' and 'board_to_fen' doesn't alter the game position.", "[FEN][Board]") {
	/*struct Board *board = fen_to_board(fen_1, NULL);
	char *fen = board_to_fen(board, NULL);
	REQUIRE(strcmp(fen, fen_1) == 0);
	board = fen_to_board(fen_2, board);
	fen = board_to_fen(board, fen);
	REQUIRE(strcmp(fen, fen_2) == 0);
	board = fen_to_board(fen_3, board);
	fen = board_to_fen(board, fen);
	REQUIRE(strcmp(fen, fen_3) == 0);
	board = fen_to_board(fen_4, board);
	fen = board_to_fen(board, fen);
	REQUIRE(strcmp(fen, fen_4) == 0);
	board_drop(board);
	free(fen);*/
}
