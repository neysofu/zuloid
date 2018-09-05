#include <stdio.h>
#include <string.h>
#include "clock.h"
#include "cmd.h"
#include "engine.h"
#include "engine_calls.h"
#include "fen.h"
#include "move.h"
#include "move_stack.h"
#include "mover.h"

void
engine_call_debug(struct Engine *engine) {
	char *debug_value = cmd_arg_ith(engine->cmd, 1);
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
engine_call_go(struct Engine *engine) {
	if (engine->status != STATUS_WAIT) {
		fprintf(stderr, "error \"Z64C must first be in idle mode to start "
				"searching.\"\n");
		return;
	}
	char *param_name;
	char *param_value;
	size_t i = 1;
	while ((param_name = cmd_arg_ith(engine->cmd, i++))) {
		param_value = cmd_arg_ith(engine->cmd, i++);
		if (strcmp(param_name, "ponder") == 0) {
			engine->settings.ponder = true;
		} else if (strcmp(param_name, "wtime") == 0) {
			engine->settings.time_control[COLOR_WHITE]->time_available_msec = atoi(param_value);
		} else if (strcmp(param_name, "btime") == 0) {
			engine->settings.time_control[COLOR_WHITE]->time_available_msec = atoi(param_value);
		} else if (strcmp(param_name, "winc") == 0) {
			engine->settings.time_control[COLOR_WHITE]->time_increment_msec = atoi(param_value);
		} else if (strcmp(param_name, "binc") == 0) {
			engine->settings.time_control[COLOR_BLACK]->time_increment_msec = atoi(param_value);
		} else if (strcmp(param_name, "depth") == 0) {
			engine->settings.max_depth = atoi(param_value);
		} else if (strcmp(param_name, "nodes") == 0) {
			engine->settings.max_num_nodes = atoi(param_value);
		} else if (strcmp(param_name, "movetime") == 0) {
			engine->settings.time_control[COLOR_WHITE]->time_available_msec = atoi(param_value);
		} else if (strcmp(param_name, "infinite") == 0) {
			engine->settings.max_num_nodes = 0;
		}
	}
	// TODO
}

void
engine_call_help(struct Engine *engine) {
	FILE *help_txt = fopen(saves_dir_help_path(engine->saves_dir), "r");
	if (help_txt) {
		printf("List of available commands:\n");

		fclose(help_txt);
	}
	// TODO: print all available commands.
}

void
engine_call_is_legal(struct Engine *engine) {
	char *move_as_str = cmd_arg_ith(engine->cmd, 1);
	Move move = str_to_move(move_as_str);
	mover_set_up(&engine->mover, &engine->board, move);
	if (mover_approves_legal_move(&engine->mover)) {
		printf("yes\n");
	} else {
		printf("no\n");
	}
}

void
engine_call_purge(struct Engine *engine) {
	ttable_purge(engine->ttable);
	// TODO: reset clocks.
}

void
engine_call_move(struct Engine *engine) {
	if (engine->status != STATUS_WAIT) {
		fprintf(stderr, "error \"Z64C must be in idle mode before making "
			   "changes to the chess position.\"\n");
		return;
	}
	char *move_as_str = cmd_arg_ith(engine->cmd, 1);
	Move move = str_to_move(move_as_str);
	mover_set_up(&engine->mover, &engine->board, move);
	if (move == MOVE_NONE) {
		fprintf(stderr, "error \"Moves must be in coordinate notation.\"\n");
	} else if (mover_approves_legal_move(&engine->mover)) {
		mover_push_legal_move(&engine->mover);
		move_stack_push(engine->move_stack, move);
	} else {
		fprintf(stderr, "error \"%s is illegal.\"\n", move_as_str);
	}
}

void
engine_call_perft(struct Engine *engine) {
	Move legal_moves[256];
	size_t counter = 1;
	size_t depth = atoi(cmd_arg_ith(engine->cmd, 1));
	board_legal_moves(&engine->board, &legal_moves);
	for (counter++) {
		ttable_insert(&engine->board_buffer);
	}
}

void
engine_call_print(struct Engine *engine) {
	char *variable = cmd_arg_ith(engine->cmd, 1);
	// Metadata
	if (strcmp(variable, "name") == 0) {
		printf("%s\n", ENGINE_NAME);
	} else if (strcmp(variable, "version") == 0) {
		printf("%s\n", ENGINE_VERSION);
	} else if (strcmp(variable, "release-date") == 0) {
		printf("%s\n", ENGINE_RELEASE_DATE);
	} else if (strcmp(variable, "author") == 0) {
		printf("%s\n", ENGINE_AUTHOR);
	} else if (strcmp(variable, "website") == 0) {
		printf("%s\n", ENGINE_WEBSITE);
	// Board position
	} else if (strcmp(variable, "position") == 0) {
		board_print(&engine->board);
	} else if (strcmp(variable, "position:castling") == 0) {
		if (board_castling_rights(&engine->board) &
			GAME_STATE_CASTLING_RIGHT_WK) {
			putchar('K');
		}
		if (board_castling_rights(&engine->board) &
			GAME_STATE_CASTLING_RIGHT_BK) {
			putchar('k');
		}
		if (board_castling_rights(&engine->board) &
			GAME_STATE_CASTLING_RIGHT_WQ) {
			putchar('Q');
		}
		if (board_castling_rights(&engine->board) &
			GAME_STATE_CASTLING_RIGHT_BQ) {
			putchar('q');
		}
		putchar('\n');
	} else if (strcmp(variable, "position:turn") == 0) {
		if (board_active_color(&engine->board) == COLOR_WHITE) {
			printf("white\n");
		} else {
			printf("black\n");
		}
	} else if (strcmp(variable, "position:en-passant") == 0) {
	} else if (strcmp(variable, "position:fen") == 0) {
		char *fen = board_to_fen(&engine->board, NULL);
		printf("%s\n", fen);
		free(fen);
	} else if (strcmp(variable, "pgn") == 0) {
	} else if (strcmp(variable, "setting:debug") == 0) {
		printf("%s\n", engine->settings.debug ? "yes" : "no");
	} else if (strcmp(variable, "setting:ponder") == 0) {
		printf("%s\n", engine->settings.ponder ? "yes" : "no");
	} else if (strcmp(variable, "setting:train") == 0) {
		printf("%s\n", engine->settings.train ? "yes" : "no");
	} else if (strcmp(variable, "setting:use-time-control") == 0) {
		printf("%s\n", engine->settings.use_time_control ? "yes" : "no");
	} else if (strcmp(variable, "setting:port") == 0) {
		printf("%u\n", engine->settings.port);
	} else if (strcmp(variable, "setting:max-cache-size-in-bytes") == 0) {
		printf("%llu\n", engine->settings.max_cache_size_in_bytes);
	} else if (strcmp(variable, "setting:move-selection-noise") == 0) {
		printf("%.3f\n", engine->settings.move_selection_noise);
	} else {
		printf("error \"'%s' is not a valid value.\"\n", variable);
	}
}

void
engine_call_set(struct Engine *engine) {
	char *name = cmd_arg_ith(engine->cmd, 1);
	char *value = cmd_arg_ith(engine->cmd, 2);
	if (!name || !value) {
		return;
	}
	if (strcmp(name, "debug") == 0) {
		//engine->settings.debug = atof(value);
	} else if (strcmp(name, "ponder") == 0) {
		//engine->settings.debug = atof(value);
	} else if (strcmp(name, "train") == 0) {
		//engine->settings.debug = atof(value);
	} else if (strcmp(name, "use-time-control") == 0) {
		//engine->settings.debug = atof(value);
	} else if (strcmp(name, "port") == 0) {
		engine->settings.debug = atoi(value);
	} else if (strcmp(name, "max-cache-size-in-bytes") == 0) {
		engine->settings.debug = atoi(value);
	} else if (strcmp(name, "move-selection-noise") = 0) {
		engine->settings.move_selection_noise = atof(value);
	} else if (strcmp(name, "resign-rate") = 0) {
		engine->settings.resign_rate = atof(value);
	} else if (strcmp(name, "resoluteness") == 0) {
		engine->settings.resoluteness = atof(value);
	} else if (strcmp(name, "selectivity") == 0) {
		engine->settings.selectivity = atof(value);
	} else if (strcmp(name, "move-selection-noise") = 0) {
		engine->settings.move_selection_noise = atof(value);
	} else if (strcmp(name, "move-selection-noise") = 0) {
		engine->settings.move_selection_noise = atof(value);
	}
}

void
engine_call_setup(struct Engine *engine) {
	if (engine->status != STATUS_WAIT) {
		return;
	}
	char *fen = cmd_arg_ith(engine->cmd, 1);
	char *move_as_str;
	if (!fen) {
		engine->board = BOARD_STARTPOS;
		return;
	} else if (strcmp(fen, "chess960") == 0) {
		// TODO
		//engine->board = board_new();
	} else {
		fen_to_board(cmd_arg_ith(engine->cmd, 2), &engine->board);
	}
	//while ((move_as_str = cmd_arg_ith(&engine->cmd))) {
	//	Move move = str_to_move(move_as_str);
	//	if (!mover_approves_legal_move(&engine->mover)) {
	//		fprintf(stderr,
	//				"error \"One or more of the given moves are "
	//				"illegal (%s).\"\n",
	//				move_as_str);
	//		return;
	//	}
	//	mover_set_up(&engine->mover, &engine->board, str_to_move(move_as_str));
	//	engine->result = mover_push_legal_move(&engine->mover);
	//}
}

void
engine_call_think(struct Engine *engine) {
	if (engine->status != STATUS_SEARCH) {
		fprintf(stderr,
			    "error \"Z64C can't stop the search because it wasn't "
			    "searching in the first place.\"\n");
		return;
	}
	Move best_move = ttable_best_move(engine->ttable);
	// TODO
	//char *move_as_str = move_to_str(best_move, );
	//char *move_as_str = "bestmove        \n";
	//move_to_str(move_as_str + 9)
	//free(move_as_str);
}
