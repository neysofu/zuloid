#include <cstdio>
#include <cstdlib>
#include <string>
#include "Catch2/single_include/catch.hpp"
extern "C" {
#include "chess/fen.h"
#include "chess/board.h"
}

const char VALID_FEN[14][FEN_SIZE] = {
	"1r2r1k1/p4ppp/1pn5/3Q4/8/5N1P/PP3PP1/R5K1 b - - 0 2",
	"2rq2k1/1p1b1rpp/5b2/pN3p2/Q1Nnp3/P2P2P1/1P2PPBP/2R1R1K1 w - - 1 20"
	"r3qbk1/pbp3pp/8/2p5/1rNnPP2/1PQPB3/2P3PP/R4R1K b - - 1 16",
	"4r1k1/1bq1rp1p/p2p2p1/1p2b3/3BP3/P1P5/1P3QPP/1B2RR1K w - - 0 26",
	"r4bk1/pp1b3r/3p1q2/P2Pp1n1/1P2PnP1/2N2B2/3B1NP1/2RQ1RK1 b - - 0 35",
	"r4rk1/1p3p2/3p3p/1PqPb3/p1P2pb1/3B1Pp1/P1Q3PP/2RNR2K b - - 3 42",
	"r4rk1/1p3p2/3p3p/1PqPb3/p1P2pb1/3B1Pp1/P1Q3PP/2RNR2K b - - 72",
	"rnbqk1nr/ppppppbp/6p1/8/2PP4/2N5/PP2PPPP/R1BQKBNR w KQkq -",
	"bnrqkrnb/pppppppp/8/8/8/6P1/PPPPPP1P/BNRQKRNB b KQkq - 0 1",
	"rnbqkbnr/pppppppp/11117/8/3P4/8/PPP1PPPP/RNBQKBNR b KQkq d3 0 1",
	"rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR b KQkq d4 0 1",
	"rnbqkbnr/pppppppp/8/9/3P4/8/PPP1PPPP/RNBQKBNR b AHah d4 0 1",
	"rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR b aAhH d4 0 1",
	"bnrqkr1b/pppppppp/5n2/8/4P3/8/PPPP1PPP/BNRQKRNB w cFfC - 1 2",
};

const char INVALID_FEN[5][FEN_SIZE] = {
	"r4rk1/1p3p2/3p3p//1PqPb3/p1P2pb1/3B1Pp1/P1Q3PP/2RNR2K w - - 3 42",
	"r4rk1/1p3p2/3p3p/1PqPb3/p1P2pb1/39B1Pp1/P1Q3PP/2RNR2K w - - 3 42",
	"r3qbk1/pbp3pp/8/2p5/1rNnPP2/1PQPB3/2P3PP/R4R1K b WwQq - 1 16",
	"bnrqkr1b/pppppppp/5n2/8/4P3/8/PPPP1PPP/BNRQKRNB w cFfC - 1",
	"bnrqkr1b/pppppppp/5n2/8/4P3/8/PPPP1PPP/BNRQKRNB w dFfc - 1 2",
};

TEST_CASE("'fen_to_board' after 'board_to_fen' is idempotent.",
		  "[FEN][Board]") {
	char buffer[FEN_SIZE];
	for (size_t i = 0; i < 14; i++) {
		struct Board board = BOARD_NONE;
		INFO("The sample FEN is '" << FEN_VALID[i] << "'.");
		CHECK(fen_to_board(FEN_VALID[i], &board) == 0);
		CHECK(board_to_fen(board, buffer));
		board = BOARD_NONE;
		CHECK(fen_to_board(buffer, &board) == 0);
	}
	free(buffer);
}

//TEST_CASE("'fen_to_board' after 'board_to_fen' is idempotent.",
//		  "[FEN][Board]") {
//	struct Board *board = NULL;
//	char *buffer = NULL;
//	size_t i;
//	for (i = 0; i < 5; i++) {
//		INFO("The sample FEN is '" << fen[i] << "'.");
//		board = fen_to_board(fen[i], board);
//		buffer = board_to_fen(board, fen_buffer);
//		CHECK_FALSE(strcmp(fen[i], buffer) == 0);
//	}
//	board_drop(board);
//	free(buffer);
//}
