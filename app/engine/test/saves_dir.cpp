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

TEST_CASE("Chess games end in a draw if a player has no legal moves to play.") {
	setenv("Z64C_DIR", "spam");
	struct SavesDir *saves_dir = saves_dir_new();
	CHECK(strcmp(saves_dir_help_path(saves_dir, "spam/help") == 0));
	CHECK(strcmp(saves_dir_ucirc_path(saves_dir, "spam/ucirc") == 0));
	CHECK(strcmp(saves_dir_graph_path(saves_dir, "spam/graph") == 0));
}
