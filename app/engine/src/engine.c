#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sys/select.h>
#include <sysexits.h>
#include <unistd.h>
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
	if (!(engine->ttable = ttable_new())) {
		goto drop_ttable;
	}
	engine->board = BOARD_STARTPOS;
	engine->result = RESULT_NONE;
	if (!(engine->move_stack = move_stack_new())) {
		goto drop_move_stack;
	}
	if (!(engine->cmd = cmd_read_str(NULL, ""))) {
		goto drop_cmd;
	}
	engine->status = STATUS_WAIT;
	engine->settings = settings_default();
	if (!(engine->saves_dir = saves_dir_new())) {
		goto drop_saves_dir;
	}
	return engine;
drop_saves_dir:
	saves_dir_drop(engine->saves_dir);
drop_cmd:
	cmd_drop(engine->cmd);
drop_move_stack:
	move_stack_drop(engine->move_stack);
drop_ttable:
	ttable_drop(engine->ttable);
	free(engine);
	return NULL;
}

void
engine_drop(struct Engine *engine) {
	ttable_drop(engine->ttable);
	move_stack_drop(engine->move_stack);
	saves_dir_drop(engine->saves_dir);
	free(engine);
}

void
engine_load_graph(struct Engine *engine) {
	FILE* file;
	if (!(file = fopen(saves_dir_graph_path(engine->saves_dir), "rb"))) {
		fseek(file, 0, SEEK_END);
		size_t file_size = ftell(file);
		fseek(file, 0, SEEK_SET);
		void *file_contents = malloc(file_size);
		fclose(file);
	}
}

uint_fast8_t
engine_main(struct Engine *engine) {
	FILE *ucirc;
	if ((ucirc = fopen(saves_dir_ucirc_path(engine->saves_dir), "r"))) {
		while ((engine->cmd = cmd_read_stream(engine->cmd, ucirc))) {
			engine_call(engine);
		}
		fclose(ucirc);
	}
	printf("Welcome to the Z64C chess engine!\n"
		   "You can see a list of available commands "
		   "by typing 'HELP'.\n");
	while (engine->status != STATUS_QUIT) {
		if (!(engine->cmd = cmd_read_stream(engine->cmd, stdin))) {
			return EX_IOERR;
		}
		engine_call(engine);
	}
	return EX_OK;
}

void
engine_call(struct Engine *engine) {
	char *cmd_name = cmd_arg_ith(engine->cmd, 0);
	if (!cmd_name || cmd_name[0] == SWITCH_COMMENT_CHAR) {
		return;
	} else if (strcmp(cmd_name, "foo") == 0) {
		printf("bar\n");
	} else if (strcmp(cmd_name, "help") == 0) {
		engine_call_help(engine);
	} else if (strcmp(cmd_name, "load") == 0) {
		engine_call_load(engine);
	} else if (strcmp(cmd_name, "move") == 0) {
		engine_call_move(engine);
	} else if (strcmp(cmd_name, "print") == 0) {
		engine_call_print(engine);
	} else if (strcmp(cmd_name, "save") == 0) {
		engine_call_save(engine);
	} else if (strcmp(cmd_name, "set") == 0) {
		engine_call_set(engine);
	} else if (strcmp(cmd_name, "setup") == 0) {
		engine_call_setup(engine);
	} else if (strcmp(cmd_name, "quit") == 0) {
		engine->status = STATUS_QUIT;
	} else if (strcmp(cmd_name, "think") == 0) {
		engine_call_think(engine);
#if Z64C_SWITCH_TRAIN
	} else if (strcmp(cmd_name, "train") == 0) {
		engine_call_train(engine);
#endif
	} else {
		fprintf(stderr,
				"error \"'%s' is not a known UCI command.\"\n", cmd_name);
	}
}
