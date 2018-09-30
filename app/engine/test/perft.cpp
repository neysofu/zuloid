#include <cstdio>
#include <cstdlib>
#include <string>
#include "catch.hpp"
extern "C" {
#include <sysexits.h>
#include "board.h"
#include "cmd.h"
#include "move.h"
#include "mover.h"
}

TEST_CASE("Perft results are verified.", "[Board][perft][FEN]") {
	char *str;
	char *perft;
	size_t str_length;
	FILE *file = open_memstream(&str, &str_length);
	struct driver *driver = driver_new();
	driver_run_str(driver, "setup 'n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1'", stdout);
	driver_run_str(driver, "perft 5", file);
	getline(&perft, &str_length, file);
	cmd = cmd_read_stream(cmd, file);
	CHECK(strstr(perft, "3797944"));

	driver_run_str(driver,
			   "setup 'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR "
			   "w KQkq - 0 1 '",
			   stdout);
	driver_run_str(driver, file, "perft 0");
	cmd = cmd_read_stream(cmd, file);
	CHECK(strcmp(cmd_arg_ith(cmd, 0), "1"));

	driver_run_str(driver,
			"setup 'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR "
			"w KQkq - 0 1 '",
			stdout);
	driver_run_str(driver, "perft 5", file);
	cmd = cmd_read_stream(cmd, file);
	CHECK(strcmp(cmd_arg_ith(cmd, 0), "4865609"));

	driver_run_str(driver,
			   "setup 'r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R "
			   "w KQkq -'",
			   stdout);
	driver_run_str(driver, file, "perft 2");
	cmd = cmd_read_stream(cmd, file);
	CHECK(strcmp(cmd_arg_ith(cmd, 0), "2039"));

	driver_run_str(driver,
			   "setup 'r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R "
			   "w KQkq -'");
	driver_run_str(driver, file, "perft 4");
	cmd = cmd_read_stream(cmd, file);
	CHECK(strcmp(cmd_arg_ith(cmd, 0), "4085603"));

	driver_run_str(driver,
			       "setup '8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - '");
	driver_run_str(driver, file, "perft 5");
	cmd = cmd_read_stream(cmd, file);
	CHECK(strcmp(cmd_arg_ith(cmd, 0), "674624"));

	driver_run_str(driver,
			   "setup 'r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 "
			   "w kq - 0 1'");
	driver_run_str(driver, file, "perft 4");
	cmd = cmd_read_stream(cmd, file);
	CHECK(strcmp(cmd_arg_ith(cmd, 0), "422333"));

	driver_run_str(driver,
			   "setup 'rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R "
			   "w KQ - 1 8'");
	driver_run_str(driver, file, "perft 1");
	cmd = cmd_read_stream(cmd, file);
	CHECK(strcmp(cmd_arg_ith(cmd, 0), "44"));

	driver_run_str(driver,
			   "setup 'rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R "
			   "w KQ - 1 8'");
	driver_run_str(driver, file, "perft 5");
	cmd = cmd_read_stream(cmd, file);
	CHECK(strcmp(cmd_arg_ith(cmd, 0), "89941194"));

	driver_run_str(driver,
				   "setup 'r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 "
			   	   "w - - 0 10'");
	driver_run_str(driver, file, "perft 4");
	cmd = cmd_read_stream(cmd, file);
	CHECK(strcmp(cmd_arg_ith(cmd, 0), "3894594"));

	driver_drop(driver);
}
