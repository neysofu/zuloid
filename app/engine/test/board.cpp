#include <cstdio>
#include <cstdlib>
#include <string>
#include "catch.hpp"
extern "C" {
#include <sysexits.h>
#include "engine.h"
#include "board.h"
#include "board.h"
#include "color.h"
#include "move.h"
#include "settings.h"
}

const char fen_1[] = "r3qbk1/pbp3pp/8/2p5/1rNnPP2/1PQPB3/2P3PP/R4R1K b - -";
const char fen_2[] = "4r1k1/1bq1rp1p/p2p2p1/1p2b3/3BP3/P1P5/1P3QPP/1B2RR1K w - - 0 26";
const char fen_3[] = "r4bk1/pp1b3r/3p1q2/P2Pp1n1/1P2PnP1/2N2B2/3B1NP1/2RQ1RK1 b - -";
const char fen_4[] = "r4rk1/1p3p2/3p3p/1PqPb3/p1P2pb1/3B1Pp1/P1Q3PP/2RNR2K b - -";

TEST_CASE("'board_new' allocates memory when necessary." "[Board]") {
	struct Board *board = board_new(NULL);
	REQUIRE(board);
	board_drop(board);
}

TEST_CASE("'board_new' is idempotent.", "[Board]") {
	struct Board *board = board_new(NULL);
	REQUIRE(board == board_new(board));
	board_drop(board);
}

TEST_CASE("The Fegatello attack is a sequence of legal moves.", "[Board]") {
	struct Board *board = board_new(NULL);
	struct Move moves[] = {
		str_to_move("e2e4"),
		str_to_move("e7e5"),
		str_to_move("g1f3"),
		str_to_move("b8c6"),
		str_to_move("f1c4"),
		str_to_move("g8f6"),
		str_to_move("f3g5"),
		str_to_move("d7d5"),
		str_to_move("e4d5"),
		str_to_move("f6d5"),
		str_to_move("g5f7"),
		str_to_move("e8f7"),
	};
	uint8_t i = 0;
	while (i < 12) {
		REQUIRE(move_is_legal(moves[i], board));
		board_push(board, moves[i++]);
	}
}
