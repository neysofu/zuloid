#include <cstdio>
#include <cstdlib>
#include <string>
#include "catch.hpp"
extern "C" {
#include <sysexits.h>
#include "engine.h"
#include "fen.h"
#include "move.h"
#include "board.h"
#include "color.h"
#include "settings.h"
}

//TEST_CASE("Z64C agrees on python-chess whether or not a move is legal.",
//		  "[Board][legality]") {
//	settings_refresh();
//	struct Board *board = board_new(NULL);
//	size_t filename_len = 512;
//	char *filename = (char *)(malloc(filename_len));
//	snprintf(filename, filename_len, "%s/illegal.fen", Z64C_TEST_DIR);
//	FILE *file = fopen(filename, "r");
//	REQUIRE(file);
//	bool flag_new_position;
//	char *line = (char *)(malloc(FEN_MAX_LENGTH));
//	size_t line_len = FEN_MAX_LENGTH;
//	while (getline(&line, &line_len, file) != -1) {
//		if (flag_new_position) {
//			flag_new_position = false;
//			board = fen_to_board(line, board);
//		} else if ((flag_new_position = isspace(line[0]))) {
//			continue;
//		} else {
//			struct Move move = str_to_move(line);
//			INFO("The position is '" << board_to_fen(board, line) << "'.");
//			move_to_str(move, line); // Removes newline.
//			INFO("The move being tested is '" << line << "'.");
//			CHECK(!move_is_legal(move, board));
//		}
//	}
//	board_drop(board);
//	free(filename);
//	fclose(file);
//	free(line);
//}
//
//TEST_CASE("One player cannot move twice in a row.",
//		  "[Board][legality]") {
//	struct Board *board = board_new(NULL);
//	REQUIRE(board->active_color == COLOR_WHITE);
//	board_push(board, str_to_move("e2e4"));
//	REQUIRE(board->active_color == COLOR_BLACK);
//	board_drop(board);
//}
