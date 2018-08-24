#include <stdio.h>
#include <string.h>
#include "clock.h"
#include "cmd.h"
#include "engine.h"
#include "engine_calls.h"
#include "fen.h"

void
engine_call_debug(struct Engine *engine) {
	char *debug_value = cmd_next(engine->cmd);
	if (!debug_value || strcmp(debug_value, "true")) {
		engine->settings.debug = true;
	} else if (strcmp(debug_value, "false") == 0) {
		engine->settings.debug = false;
	} else {
		fprintf(stderr, "error \"The debug setting can only be set to either "
				"'true' or 'false'.\"\n");
	}
}

void
engine_call_set_option(struct Engine *engine) {
	char *name_flag = cmd_next(engine->cmd);
	if (!name_flag || strcmp(name_flag, "name") != 0) {
		return;
	}
	// TODO
}

void
engine_call_go(struct Engine *engine) {
	if (engine->status != STATUS_WAIT) {
		fprintf(stderr, "error \"Z64C must first be in idle mode to start "
				"searching.\"\n");
		return;
	}
	char *param_name;
	char *param_value;
	while ((param_name = cmd_next(engine->cmd))) {
		param_value = cmd_next(engine->cmd);
		if (strcmp(param_name, "ponder") == 0) {
			engine->settings.ponder = true;
		} else if (strcmp(param_name, "wtime") == 0) {
			engine->settings.clocks[COLOR_WHITE]->time_available_msec = atoi(param_value);
		} else if (strcmp(param_name, "btime") == 0) {
			engine->settings.clocks[COLOR_WHITE]->time_available_msec = atoi(param_value);
		} else if (strcmp(param_name, "winc") == 0) {
			engine->settings.clocks[COLOR_WHITE]->time_increment_msec = atoi(param_value);
		} else if (strcmp(param_name, "binc") == 0) {
			engine->settings.clocks[COLOR_BLACK]->time_increment_msec = atoi(param_value);
		} else if (strcmp(param_name, "depth") == 0) {
			engine->settings.max_depth = atoi(param_value);
		} else if (strcmp(param_name, "nodes") == 0) {
			engine->settings.max_num_nodes = atoi(param_value);
		} else if (strcmp(param_name, "movetime") == 0) {
			engine->settings.clocks[COLOR_WHITE]->time_available_msec = atoi(param_value);
		} else if (strcmp(param_name, "infinite") == 0) {
			engine->settings.max_num_nodes = 0;
		}
	}
	engine_search(engine);
}

void
engine_call_help(struct Engine *engine) {
	FILE *help_txt = fopen(saves_dir_help_path(engine->saves_dir));
	if (help_txt) {
		printf("List of available commands:\n");

		fclose(help_txt);
	}
	// TODO: print all available commands.
}

void
engine_call_is_legal(struct Engine *engine) {
	char *move_as_str = cmd_next(engine->cmd);
	Move move = str_to_move(move_as_str);
	if (move_is_legal(move, &engine->board)) {
		printf("yes\n");
	} else {
		printf("no\n");
	}
}

void
engine_call_new_game(struct Engine *engine) {
	// TODO: clear the hash table and reset clocks.
}

void
engine_call_move(struct Engine *engine) {
	if (engine->status != STATUS_WAIT) {
		fprintf(stderr, "error \"Z64C must be in idle mode before making "
			   "changes to the chess position.\"\n");
		return;
	}
	char *move_as_str = cmd_next(engine->cmd);
	Move move = str_to_move(move_as_str);
	if (move == MOVE_NONE) {
		fprintf(stderr, "error \"Moves must be in coordinate notation.\"\n");
	} else if (move_is_legal(move, &engine->board)) {
		board_push(&engine->board, move);
		move_stack_push(engine->move_stack, move);
	} else {
		fprintf(stderr, "error \"%s is illegal.\"\n", move_as_str);
	}
}

void
engine_call_position(struct Engine *engine) {
	if (engine->status != STATUS_WAIT) {
		return;
	}
	char *init_mode = cmd_next(engine->cmd);
	char *move_as_str;
	if (!init_mode) {
		return;
	}
	if (strcmp(init_mode, "fen") == 0) {
		cmd_join(engine->cmd, 6);
		fen_to_board(cmd_next(engine->cmd), &engine->board);
		cmd_disjoin(engine->cmd, 6);
	} else if (strcmp(init_mode, "startpos") == 0) {
		engine->board = BOARD_STARTPOS;
	}
	while ((move_as_str = cmd_next(engine->cmd))) {
		Move move = str_to_move(move_as_str);
		if (!move_is_legal(move, &engine->board)) {
			fprintf(stderr,
					"error \"One or more of the given moves are "
					"illegal (%s).\"\n",
					move_as_str);
			return;
		}
		engine->result = board_push(&engine->board, str_to_move(move_as_str));
	}
}

void
engine_call_ponder_hit(struct Engine *engine) {
	Move best_move = ttable_best_move(engine->ttable);
	engine->result = board_push(&engine->board, best_move);
	ttable_prune_other_moves(best_move);
}

void
engine_call_stop(struct Engine *engine) {
	if (engine->status != STATUS_SEARCH) {
		fprintf(stderr,
			    "error \"Z64C can't stop the search because it wasn't "
			    "searching in the first place.\"\n");
		return;
	}
	Move best_move = ttable_best_move(engine->ttable);
	char *move_as_str = move_to_str(best_move);
	char *move_as_str = "bestmove        \n";
	move_to_str(move_as_str + 9)
	free(move_as_str);
}

void
engine_call_uci(struct Engine *engine) {
	if (engine->status != STATUS_BOOT) {
		return;
	}
	printf("id name %s\n", ENGINE_NAME);
	printf("id author %s\n", ENGINE_AUTHOR);
	printf("id version %s\n", ENGINE_VERSION);
	printf("id website %s\n", ENGINE_WEBSITE);
	engine->status = STATUS_WAIT;
	printf("uciok\n");
}
