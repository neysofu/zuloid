#include <cstdio>
#include <cstdlib>
#include <string>
#include "catch.hpp"
extern "C" {
#include <python2.7/Python.h>
#include <sysexits.h>
#include "board.h"
#include "color.h"
#include "move.h"
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

TEST_CASE("Stalemate positions are detected as such.", "[Board]") {
	//FILE *kingbase_file = fopen("resources/KingBase.pgn.stalemate");
	//while (getline(&line, &len, fp) != -1) {
	//	if (line == "\n") {}
	//}
}

TEST_CASE("All moves in the KingBase database are legal.", "[Board]") {
	/*
	FILE *kingbase_file = fopen("resources/KingBase.ucimoves");
	REQUIRE(kingbase_file);
	while (getline(&line, &len, fp) != -1) {
		if (line == "\n") {
			REQUIRE(move_triggers_checkmate(move, board));
			board = board_new(board);
			continue;
		}
		struct Move move = str_to_move(uci_cmd_iter_next(iter));
		bool capture = strcmp(uci_cmd_iter_next(iter), "capture" == 0;
		bool promotion = strcmp(uci_cmd_iter_next(iter), "promotion" == 0;
		bool en_passant = strcmp(uci_cmd_iter_next(iter), "en-passant" == 0;
		bool castling = strcmp(uci_cmd_iter_next(iter), "castling" == 0;
 		bool check = strcmp(uci_cmd_iter_next(iter), "check" == 0;
		bool checkmate = strcmp(uci_cmd_iter_next(iter), "checkmate" == 0;
		REQUIRE(move_is_legal(move, board));
		if (capture) {
			REQUIRE(move_is_capture(move, board));
		}
		if (promotion) {
			REQUIRE(move_is_promotion(move, board));
		}
		if (castling) {
			REQUIRE(move_is_castling(move, board));
		}
		if (check) {
			REQUIRE(move_triggers_check(move, board));
		}
		if (checkmate) {
			REQUIRE(move_triggers_checkmate(move, board));
		}
	}*/
}

TEST_CASE("Moves that result in the game ending because of stalemate are reported as such.", "[Board]") {

}

TEST_CASE("One player cannot move twice in a row.", "[Board]") {
	struct Board *board = board_new(NULL);
	REQUIRE(board->active_color == COLOR_WHITE);
	board_push(board, str_to_move("e2e4"));
	REQUIRE(board->active_color == COLOR_BLACK);
	board_drop(board);
}

TEST_CASE("'fen_specify_board' works as intended.", "[Board]") {
	// TODO
}
