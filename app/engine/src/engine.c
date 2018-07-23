#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sysexits.h>
#include <sys/select.h>
//#include <tensorflow/tensorflow/c/c_api.h>
#include "engine.h"
#include "clock.h"
#include "utils.h"
#include "fen.h"
#include "board.h"
#include "search.h"
#include "settings.h"
#include "result.h"

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
	struct SearchParams *search_params;
	struct SearchPriorityQueue *search_priority_queue;
	struct Result result;
	//TF_Graph* graph;
};

struct Engine *
engine_new(struct Engine *engine) {
	if (!engine) {
		engine = malloc(sizeof(struct Engine));
		if (!engine) {
			return NULL;
		}
	}
	engine->status = STATUS_BOOT;
	engine->debug = false;
	engine->board = board_new(NULL);
	engine->search_params = search_params_new(NULL);
	engine->search_priority_queue = search_priority_queue_new(NULL);
	engine->result = RESULT_NONE;
	//engine->graph = TF_NewGraph();
	if (!(engine->board &&
		  engine->search_params &&
		  engine->search_priority_queue)) {
		return NULL;
	}
	return engine;
}

//void
//engine_load_network(struct Engine *engine) {
//	FILE* file = fopen(Z64C_DIR, "rb");
//	fseek(file, 0, SEEK_END);
//	long file_size = ftell(file);
//	fseek(file, 0, SEEK_SET);
//	void *file_contents = malloc(file_size);
//	fread(file_contents, file_size, 1, file);
//	fclose(file);
//	TF_Buffer* buffer = TF_NewBuffer();
//	buffer->data = file_contents;
//	buffer->length = file_size;
//	buffer->data_deallocator = buffer;
//	TF_Status *status = TF_NewStatus();
//	TF_ImportGraphDefOptions *options = TF_NewImportGraphDefOptions();
//	TF_GraphImportGraphDef(engine->graph, buffer, options, status);
//	TF_DeleteImportGraphDefOptions(options);
//	if (TF_GetCode(status) != TF_OK) {
//		fprintf(stderr, "ERROR: Unable to import graph %s", TF_Message(status));
//		return;
//	}
//	TF_DeleteStatus(status);
//	TF_DeleteBuffer(buffer);
//}

void
engine_drop(struct Engine *engine) {
	board_drop(engine->board);
	search_params_drop(engine->search_params);
	//TF_DeleteGraph(engine->graph);
	free(engine);
}

int
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
		char *fen_fragment = strtok(NULL, UCI_TOKEN_SEPARATORS);
		if (!fen_fragment) {
			return;
		}
		fen_specify_pieces(fen_fragment, engine->board);
		fen_fragment = strtok(NULL, UCI_TOKEN_SEPARATORS);
		if (!fen_fragment) {
			return;
		}
		fen_specify_active_color(fen_fragment, engine->board);
		fen_fragment = strtok(NULL, UCI_TOKEN_SEPARATORS);
		if (!fen_fragment) {
			return;
		}
		fen_specify_castling_rights(fen_fragment, engine->board);
		fen_fragment = strtok(NULL, UCI_TOKEN_SEPARATORS);
		if (!fen_fragment) {
			return;
		}
		fen_specify_en_passant_file(fen_fragment, engine->board);
		fen_fragment = strtok(NULL, UCI_TOKEN_SEPARATORS);
		if (!fen_fragment) {
			return;
		}
		fen_specify_half_moves(fen_fragment, engine->board);
		fen_fragment = strtok(NULL, UCI_TOKEN_SEPARATORS);
		if (!fen_fragment) {
			return;
		}
		fen_specify_full_moves(fen_fragment, engine->board);
	} else if (strcmp(init_mode, "startpos") == 0) {
		engine->board = board_new(engine->board);
	}
	while ((move_as_str = strtok(NULL, UCI_TOKEN_SEPARATORS))) {
		struct Move move = str_to_move(move_as_str);
		if (!move_is_legal(move, engine->board)) {
			return;
		}
		engine->result = board_push(engine->board, str_to_move(move_as_str));
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
			//engine->search_params->white_clock->ex = atoi(param_value);
		} else if (strcmp(param_name, "btime") == 0) {
			//engine->clock->black_msec = atoi(param_value);
		} else if (strcmp(param_name, "winc") == 0) {
			//engine->clock->white_increment_msec = atoi(param_value);
		} else if (strcmp(param_name, "binc") == 0) {
			//engine->clock->black_increment_msec = atoi(param_value);
		} else if (strcmp(param_name, "depth") == 0) {
			//engine->search_params->max_depth = atoi(param_value);
		} else if (strcmp(param_name, "nodes") == 0) {
			//engine->search_params->max_num_nodes = atoi(param_value);
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
	printf("bestmove %s ponder %s\n",
		   move_to_str(best_move, NULL),
		   move_to_str(best_move, NULL)); // FIXME
}

void
engine_call_is_legal(struct Engine *engine) {
	char *move_as_str = strtok(NULL, UCI_TOKEN_SEPARATORS);
	struct Move move = str_to_move(move_as_str);
	bool is_legal = move_is_legal(move, engine->board);
	if (is_legal) {
		printf("1\n");
	} else {
		printf("0\n");
	}
}

void
engine_call_move(struct Engine *engine) {
	char *move_as_str = strtok(NULL, UCI_TOKEN_SEPARATORS);
	struct Move move = str_to_move(move_as_str);
	//bool is_legal = move_is_legal(move, engine->board);
	board_push(engine->board, move);
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
	} else if (strcmp(cmd_name, "islegal") == 0) {
		engine_call_is_legal(engine);
	} else if (strcmp(cmd_name, "move") == 0) {
		engine_call_move(engine);
	} else if (strcmp(cmd_name, "quit") == 0) {
		engine->status = STATUS_QUIT;
	}
}

enum Termination
engine_termination(struct Engine *engine) {
	return engine->result.termination;
}
