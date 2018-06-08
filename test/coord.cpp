#include <cstdlib>
#include <string>
#include "catch.hpp"
extern "C" {
#include <sysexits.h>
#include "board.h"
#include "piece.h"
}
/*
TEST_CASE("A knight placed at the center of the chessboard threatens 8 squares.", "[Board][Piece]") {
	struct Board *board = board_new(NULL);
	Rank rank;
	File file;
	uint8_t count;
	for (rank = 0; rank < BOARD_SIDE_LENGTH; rank++) {
		for (file = 0; file < BOARD_SIDE_LENGTH; file++) {
			if (piece_threatens_square(, board))
		}
	}
	board_drop(board);
}

TEST_CASE("The en-passant target becomes invalid if not captured instantly", "[Board][Piece][Move]") {

}*/
