#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sysexits.h>
#include <sys/select.h>
//#include <tensorflow/c/c_api.h>
#include "engine.h"
#include "clock.h"
#include "utils.h"
#include "fen.h"
#include "board.h"
#include "search.h"

#define UCI_TOKEN_SEPARATORS " \t\n\v\f\r"

enum Status {
	STATUS_BOOT,
	STATUS_WAIT,
	STATUS_SEARCH,
	STATUS_QUIT,
};

struct Engine {
	enum Status status;
	bool debug;
	struct Board *board;
	struct Clock *clock;
	struct SearchParams *search_params;
	struct SearchPriorityQueue *search_priority_queue;
};

struct Engine *
engine_new(struct Engine *engine) {
	if (!engine) {
		engine = malloc(sizeof(struct Engine));
		if (!engine) {
			return NULL;
		}
	}
	engine->board = board_new(NULL);
	engine->clock = clock_new(NULL);
	engine->debug = false;
	engine->status = STATUS_BOOT;
	engine->search_priority_queue = search_priority_queue_new(NULL);
	if (!(engine->board && engine->clock && engine->search_priority_queue)) {
		return NULL;
	}
	return engine;
}

void
engine_drop(struct Engine *engine) {
	board_drop(engine->board);
	clock_drop(engine->clock);
	free(engine);
}

uint8_t
engine_main(struct Engine *engine) {
	char *cmd = NULL;
	size_t cmd_length = 0;
	while (engine->status != STATUS_QUIT) {
		if (getline(&cmd, &cmd_length, stdin) == -1) {
			return EX_IOERR;
		}
		engine_call(engine, cmd);
	}
	return EX_OK;
}

void
engine_call_uci(struct Engine *engine) {
	if (engine->status != STATUS_BOOT) {
		return;
	}
	printf("id name %s\n", ENGINE_NAME);
	printf("id author %s\n", ENGINE_AUTHOR);
	engine->status = STATUS_WAIT;
	printf("uciok\n");
}

void
engine_call_debug(struct Engine *engine) {
	char *debug_value = strtok(NULL, UCI_TOKEN_SEPARATORS);
	if (!debug_value || strcmp(debug_value, "true") == 0) {
		engine->debug = true;
	} else if (strcmp(debug_value, "false") == 0) {
		engine->debug = false;
	}
}

void
engine_call_set_option(struct Engine *engine) {
	char *name_flag = strtok(NULL, UCI_TOKEN_SEPARATORS);
	char *name;
	char *value_flag;
	char *value;
	if (!name_flag || strcmp(name_flag, "name") != 0) {
		return;
	}
	name = strtok(NULL, UCI_TOKEN_SEPARATORS);
	// TODO
}

void
engine_call_position(struct Engine *engine) {
	char *init_mode = strtok(NULL, UCI_TOKEN_SEPARATORS);
	char *move_as_str;
	if (!init_mode) {
		return;
	}
	if (strcmp(init_mode, "fen") == 0) {
		char *fen = strtok(NULL, UCI_TOKEN_SEPARATORS);
		if (!fen) {
			return;
		}
		engine->board = fen_to_board(fen, engine->board);
	} else if (strcmp(init_mode, "startpos") == 0) {
		engine->board = board_new(engine->board);
	}
	while ((move_as_str = strtok(NULL, UCI_TOKEN_SEPARATORS))) {
		board_push(engine->board, str_to_move(move_as_str));
	}
}

void
engine_call_search(struct Engine *engine) {
	char *param_name = strtok(NULL, UCI_TOKEN_SEPARATORS);
	char *param_value;
	while (param_name) {
		param_value = strtok(NULL, UCI_TOKEN_SEPARATORS);
		if (strcmp(param_name, "ponder") == 0) {
			//engine->search_params->ponder = true;
		} else if (strcmp(param_name, "wtime") == 0) {
			engine->clock->white_msec = atoi(param_value);
		} else if (strcmp(param_name, "btime") == 0) {
			engine->clock->black_msec = atoi(param_value);
		} else if (strcmp(param_name, "winc") == 0) {
			engine->clock->white_increment_msec = atoi(param_value);
		} else if (strcmp(param_name, "binc") == 0) {
			engine->clock->black_increment_msec = atoi(param_value);
		} else if (strcmp(param_name, "depth") == 0) {
			engine->search_params->max_depth = atoi(param_value);
		} else if (strcmp(param_name, "nodes") == 0) {
			engine->search_params->max_num_nodes = atoi(param_value);
		} else if (strcmp(param_name, "movetime") == 0) {
			//engine->search_params->move_time = atoi(param_value);
		} else if (strcmp(param_name, "infinite") == 0) {
			//engine->search_params->infinite = true;
		}
	}
}

void
engine_call_stop(struct Engine *engine) {
	if (engine->status != STATUS_SEARCH) {
		return;
	}
	struct Move best_move = search_priority_queue_pop_move(engine->search_priority_queue);
	printf("bestmove %s ponder %s\n", move_to_str(best_move, NULL), move_to_str(best_move, NULL)); // FIXME
}

void
engine_call_ponder_hit(struct Engine *engine) {
	return;
}

void
engine_call(struct Engine *engine, char *cmd) {
	char *cmd_name = strtok(cmd, UCI_TOKEN_SEPARATORS);
	if (!cmd_name) {
		return;
	}
	if (strcmp(cmd_name, "uci") == 0) {
		engine_call_uci(engine);
	} else if (strcmp(cmd_name, "debug") == 0) {
		engine_call_debug(engine);
	} else if (strcmp(cmd_name, "position") == 0) {
		engine_call_position(engine);
	} else if (strcmp(cmd_name, "boardprint") == 0) {
		board_print(engine->board);
	} else if (strcmp(cmd_name, "go") == 0) {
		engine_call_search(engine);
	} else if (strcmp(cmd_name, "isready") == 0) {
		printf("readyok\n");
	} else if (strcmp(cmd_name, "stop") == 0) {
		engine_call_stop(engine);
	} else if (strcmp(cmd_name, "quit") == 0) {
		engine->status = STATUS_QUIT;
	}
}
