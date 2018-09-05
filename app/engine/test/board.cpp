#include <cstdio>
#include <cstdlib>
#include <string>
#include "catch.hpp"
extern "C" {
#include <sysexits.h>
#include "board.h"
#include "move.h"
#include "mover.h"
}

TEST_CASE("The Fegatello attack is a sequence of legal moves.", "[Board]") {
	struct Board board = BOARD_STARTPOS;
	struct Mover mover;
	Move moves[] = {
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
	int i = 0;
	while (i < 12) {
		mover_set_up(&mover, &board, moves[i]);
		REQUIRE(mover_approves_legal_move(&mover));
		REQUIRE(mover_approves_pseudolegal_move(&mover));
		mover_push_legal_move(&mover);
	}
}
