#include <cstdio>
#include <cstdlib>
#include <string>
#include "catch.hpp"
extern "C" {
#include <sysexits.h>
#include "board.h"
#include "cmd.h"
#include "color.h"
#include "engine.h"
#include "move.h"
#include "settings.h"
}

const char fen_1[] = "r3qbk1/pbp3pp/8/2p5/1rNnPP2/1PQPB3/2P3PP/R4R1K b - -";
const char fen_2[] = "4r1k1/1bq1rp1p/p2p2p1/1p2b3/3BP3/P1P5/1P3QPP/1B2RR1K w - - 0 26";
const char fen_3[] = "r4bk1/pp1b3r/3p1q2/P2Pp1n1/1P2PnP1/2N2B2/3B1NP1/2RQ1RK1 b - -";
const char fen_4[] = "r4rk1/1p3p2/3p3p/1PqPb3/p1P2pb1/3B1Pp1/P1Q3PP/2RNR2K b - -";

TEST_CASE("Chess games end in a draw if a player has no legal moves to play.",
		  "[Board]") {
	struct Engine *engine = engine_new();
	settings_refresh();
	size_t buffer_size = 512;
	char *buffer = (char *)(malloc(buffer_size));
	snprintf(buffer, buffer_size, "%s/stalemate.uci", Z64C_TEST_DIR);
	FILE *uci_file = fopen(buffer, "r");
	REQUIRE(uci_file);
	while (cmd_read(engine->cmd, uci_file) != -1) {
		// FIXME
		engine_call(engine);
		REQUIRE(engine->result.termination == TERMINATION_STALEMATE);
	}
	fclose(uci_file);
	free(buffer);
}

TEST_CASE("Chess games are considered a draw after 50 reversible moves.",
		  "[Board]") {
	settings_refresh();
	size_t buffer_size = 4096;
	char *buffer = (char *)(malloc(buffer_size));
	snprintf(buffer, buffer_size, "%s/fifty.uci", Z64C_TEST_DIR);
	FILE *uci_file = fopen(buffer, "r");
	REQUIRE(uci_file);
	struct Engine *engine = engine_new();
	while (cmd_read(engine->cmd, uci_file) != -1) {
		// FIXME
		engine_call(engine);
		REQUIRE(engine->result.termination == TERMINATION_NO_CAPTURE);
	}
	fclose(uci_file);
	free(buffer);
}

TEST_CASE("Draws by insufficient material are detected as such.", "[Board]") {
	settings_refresh();
	size_t buffer_size = 4096;
	char *buffer = (char *)(malloc(buffer_size));
	snprintf(buffer, buffer_size, "%s/insufficient.uci", Z64C_TEST_DIR);
	FILE *uci_file = fopen(buffer, "r");
	REQUIRE(uci_file);
	struct Engine *engine = engine_new();
	while (cmd_read(engine->cmd, uci_file) != -1) {
		// FIXME
		engine_call(engine);
		REQUIRE(engine->result.termination == TERMINATION_INSUFFICIENT_MATERIAL);
	}
	fclose(uci_file);
	free(buffer);
}

TEST_CASE("A chess game ends with a win when one player checkmates the other.",
		  "[Board][Termination]") {
	settings_refresh();
	size_t buffer_size = 4096;
	char *buffer = (char *)(malloc(buffer_size));
	snprintf(buffer, buffer_size, "%s/checkmate.uci", Z64C_TEST_DIR);
	FILE *uci_file = fopen(buffer, "r");
	REQUIRE(uci_file);
	struct Engine *engine = engine_new();
	while (cmd_read(engine->cmd, uci_file) != -1) {
		// FIXME
		engine_call(engine);
		REQUIRE(engine->result.termination == TERMINATION_CHECKMATE);
	}
	fclose(uci_file);
	free(buffer);
}
