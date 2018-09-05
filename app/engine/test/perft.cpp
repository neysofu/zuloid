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

TEST_CASE("Perft results are verified.", "[Board][perft][FEN]") {
	struct Engine *engine = engine_new();
	engine_run(engine, "setup 'n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1'");
	perft = engine_run(engine, "perft 5");
	CHECK(strstr(perft, "3797944"));
	engine_run(engine, "setup 'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 '");
	perft = engine_run(engine, "perft 0", stdout);
	CHECK(strstr(perft, "1"));
	engine_run(engine, "setup 'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 '");
	perft = engine_run(engine, "perft 5", stdout);
	CHECK(strstr(perft, "4865609"));
	engine_run(engine, "setup 'r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -'");
	perft = engine_run(engine, "perft 2");
	CHECK(strstr(perft, "2039"));
	engine_run(engine, "setup 'r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -'");
	perft = engine_run(engine, "perft 4");
	CHECK(strstr(perft, "4085603"));
	engine_run(engine, "setup '8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - '");
	perft = engine_run(engine, "perft 5");
	CHECK(strstr(perft, "674624"));
	engine_run(engine, "setup 'r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1'");
	perft = engine_run(engine, "perft 4");
	CHECK(strstr(perft, "422333"));
	engine_run(engine, "setup 'rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8'");
	perft = engine_run(engine, "perft 1");
	CHECK(strstr(perft, "44"));
	engine_run(engine, "setup 'rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8'");
	perft = engine_run(engine, "perft 5");
	CHECK(strstr(perft, "89941194"));
	engine_run(engine, "setup 'r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10'");
	perft = engine_run(engine, "perft 4");
	CHECK(strstr(perft, "3894594"));
	engine_drop(engine);
}
