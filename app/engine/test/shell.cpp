#include <cstdio>
#include <cstdlib>
#include <string>
#include "catch.hpp"
extern "C" {
#include "cmd.h"
}

TEST_CASE("Empty UCI commands contain no tokens.", "[struct Cmd][UCI]") {
	struct Cmd *cmd = NULL;
	cmd = cmd_read_str(cmd, " \t   \n ");
	CHECK_FALSE(cmd_arg_ith(cmd, 0));
	cmd = cmd_read_str(cmd, "");
	CHECK_FALSE(cmd_arg_ith(cmd, 0));
	cmd = cmd_read_str(cmd, "\n");
	CHECK_FALSE(cmd_arg_ith(cmd, 0));
	cmd_drop(cmd);
}

TEST_CASE("UCI commands can contain tokens separated by any whitespace "
		  "characters.", "[struct Cmd][UCI]") {
	struct Cmd *cmd = NULL;
	cmd = cmd_read_str(cmd, " \t spam  \n ");
	CHECK(strcmp(cmd_arg_ith(cmd, 0), "spam") == 0);
	CHECK_FALSE(cmd_arg_ith(cmd, 1));
	cmd = cmd_read_str(cmd, "  foobar   pickle\t\trick");
	CHECK(strcmp(cmd_arg_ith(cmd, 0), "foobar") == 0);
	CHECK(strcmp(cmd_arg_ith(cmd, 1), "pickle") == 0);
	CHECK(strcmp(cmd_arg_ith(cmd, 2), "rick") == 0);
	CHECK_FALSE(cmd_arg_ith(cmd, 3));
	cmd = cmd_read_str(cmd, "\n");
	CHECK_FALSE(cmd_arg_ith(cmd, 0));
	cmd_drop(cmd);
}
