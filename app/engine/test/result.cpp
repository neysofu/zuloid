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

TEST_CASE("Stalemate positions are detected as such.", "[Board]") {
	settings_init();
	size_t buffer_size = 4096;
	char *buffer = (char *)(malloc(buffer_size));
	snprintf(buffer, buffer_size, "%s/lichess.uci.stalemate", Z64C_TEST_DIR);
	FILE *uci_file = fopen(buffer, "r");
	REQUIRE(uci_file);
	struct Engine *engine = engine_new(NULL);
	while (getline(&buffer, &buffer_size, uci_file) != -1) {
		// FIXME
		engine_call(engine, buffer);
		REQUIRE(engine_termination(engine) == TERMINATION_STALEMATE);
	}
	fclose(uci_file);
	free(buffer);
}

TEST_CASE("Chess games are considered a draw after 50 reversible moves.",
		  "[Board]") {
	settings_init();
	size_t buffer_size = 4096;
	char *buffer = (char *)(malloc(buffer_size));
	snprintf(buffer, buffer_size, "%s/lichess.uci.fifty", Z64C_TEST_DIR);
	FILE *uci_file = fopen(buffer, "r");
	REQUIRE(uci_file);
	struct Engine *engine = engine_new(NULL);
	while (getline(&buffer, &buffer_size, uci_file) != -1) {
		// FIXME
		engine_call(engine, buffer);
		REQUIRE(engine_termination(engine) == TERMINATION_NO_CAPTURE);
	}
	fclose(uci_file);
	free(buffer);
}

TEST_CASE("Checkmates are detected  such.", "[Board]") {
	settings_init();
	size_t buffer_size = 4096;
	char *buffer = (char *)(malloc(buffer_size));
	snprintf(buffer, buffer_size, "%s/lichess.uci.checkmate", Z64C_TEST_DIR);
	FILE *uci_file = fopen(buffer, "r");
	REQUIRE(uci_file);
	struct Engine *engine = engine_new(NULL);
	while (getline(&buffer, &buffer_size, uci_file) != -1) {
		// FIXME
		engine_call(engine, buffer);
		REQUIRE(engine_termination(engine) == TERMINATION_CHECKMATE);
	}
	fclose(uci_file);
	free(buffer);
}

TEST_CASE("A chess game ends with a win when one player checkmates the other.",
		  "[Board][Termination]") {
	settings_init();
	size_t buffer_size = 4096;
	char *buffer = (char *)(malloc(buffer_size));
	snprintf(buffer, buffer_size, "%s/lichess.uci.checkmate", Z64C_TEST_DIR);
	FILE *uci_file = fopen(buffer, "r");
	REQUIRE(uci_file);
	struct Engine *engine = engine_new(NULL);
	while (getline(&buffer, &buffer_size, uci_file) != -1) {
		// FIXME
		engine_call(engine, buffer);
		REQUIRE(engine_termination(engine) == TERMINATION_CHECKMATE);
	}
	fclose(uci_file);
	free(buffer);
}
