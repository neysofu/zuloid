#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sysexits.h>
#include <sys/select.h>
#include "board.h"
#include "clock.h"
#include "cmd.h"
#include "engine.h"
#include "engine_calls.h"
#include "fen.h"
#include "globals.h"
#include "move.h"
#include "move_stack.h"
#include "result.h"
#include "saves_dir.h"
#include "settings.h"
#include "settings.h"
#include "switches.h"
#include "ttable.h"
#include "utils.h"

struct Engine *
engine_new(void) {
	struct Engine *engine = malloc(sizeof(struct Engine));
	if (!engine) {
		return NULL;
	}
	engine->status = STATUS_BOOT;
	engine->cmd = cmd_from_str(NULL, "");
	engine->board = BOARD_STARTPOS;
	engine->result = RESULT_NONE;
	engine->settings = settings_default();
	engine->saves_dir = saves_dir_new();
	engine->ttable = ttable_new();
	// TODO: NULL checking.
	return engine;
}

void
engine_drop(struct Engine *engine) {
	if (engine->cmd) {
		cmd_drop(engine->cmd);
	}
	free(engine);
}


void
engine_load_graph(struct Engine *engine) {
	FILE* file = fopen(saves_dir_graph_path(engine->saves_dir), "rb");
	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	fseek(file, 0, SEEK_SET);
	void *file_contents = malloc(file_size);
	// TODO
}

uint_fast8_t
engine_main(struct Engine *engine) {
	FILE *ucirc;
	if ((ucirc = fopen(saves_dir_ucirc_path(engine->saves_dir), "r"))) {
		while (cmd_from_stream(engine->cmd, ucirc) != NULL) {
			engine_call(engine);
		}
		fclose(ucirc);
	}
	printf("Welcome to the Z64C chess engine!\n"
		   "You can see a list of available commands "
		   "by typing 'help'.\n");
	while (engine->status != STATUS_QUIT) {
		if (cmd_from_stream(engine->cmd, stdin) == NULL) {
			return EX_IOERR;
		}
		engine_call(engine);
	}
	return EX_OK;
}

void
engine_call(struct Engine *engine) {
	char *cmd_name = cmd_ith(engine->cmd, 0);
	if (!cmd_name || cmd_name[0] == Z64C_FLAG_COMMENT_CHAR) {
		return;
	}
	if (strcmp(cmd_name, "uci") == 0) {
		if (engine->status == STATUS_BOOT) {
			engine_call_uci(engine);
		} else {
			fprintf(stderr, "error \"The engine has booted already.\"\n");
		}
	} else if (strcmp(cmd_name, "boardprint") == 0) {
		board_print(&engine->board);
	} else if (strcmp(cmd_name, "debug") == 0) {
		engine_call_debug(engine);
	} else if (strcmp(cmd_name, "go") == 0) {
		engine_call_go(engine);
	} else if (strcmp(cmd_name, "help") == 0) {
		engine_call_help(engine);
	} else if (strcmp(cmd_name, "islegal") == 0) {
		engine_call_is_legal(engine);
	} else if (strcmp(cmd_name, "isready") == 0) {
		printf("readyok\n");
	} else if (strcmp(cmd_name, "move") == 0) {
		engine_call_move(engine);
	} else if (strcmp(cmd_name, "ponderhit") == 0) {
		engine_call_ponder_hit(engine);
	} else if (strcmp(cmd_name, "position") == 0) {
		engine_call_position(engine);
	} else if (strcmp(cmd_name, "quit") == 0) {
		engine->status = STATUS_QUIT;
	} else if (strcmp(cmd_name, "stop") == 0) {
		engine_call_stop(engine);
	} else if (strcmp(cmd_name, "ucinewgame") == 0) {
		engine_call_new_game(engine);
	} else {
		fprintf(stderr,
				"error \"'%s' is not a known UCI command.\"\n", cmd_name);
	}
}
