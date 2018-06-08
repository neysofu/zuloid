#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sysexits.h>
#include <sys/select.h>
#include "engine.h"
#include "transpos_table.h"
#include "utils.h"
#include "fen.h"
#include "board.h"
#include "uci.h"
#include "search.h"

enum Status {
	STATUS_BOOT,
	STATUS_WAIT,
	STATUS_QUIT,
};

struct Engine {
	struct UciCmd *cmd;
	struct UciCmdIter *cmd_iter;
	struct Board *board;
	bool is_searching;
	bool debug;
	enum Status status;
	struct TransposTable *transpos_table;
	struct SearchParams *params;
};

struct Engine *
engine_new(struct Engine *engine) {
	if (!engine) {
		engine = malloc(sizeof(struct Engine));
		if (!engine) {
			return NULL;
		}
	}
	engine->cmd = uci_cmd_new(NULL);
	engine->cmd_iter = uci_cmd_iter_new(NULL, engine->cmd, 0);
	engine->board = board_new(NULL);
	engine->is_searching = false;
	engine->debug = false;
	engine->status = STATUS_BOOT;
	engine->transpos_table = transpos_table_new(NULL);
	if (!(engine->cmd) || !(engine->board)) {
		return NULL;
	}
	return engine;
}

void
engine_drop(struct Engine *engine) {
	uci_cmd_iter_drop(engine->cmd_iter);
	uci_cmd_drop(engine->cmd);
	board_drop(engine->board);
	free(engine);
}

void
engine_wait_for_uci_cmd(struct Engine *engine) {
	size_t line_len = 0;
	if ((line_len = getline(&(engine->cmd->str), &line_len, stdin)) == -1) {
		return;
	}
	engine->cmd = str_to_uci_cmd(engine->cmd->str, line_len, engine->cmd);
	engine->cmd_iter = uci_cmd_iter_new(engine->cmd_iter, engine->cmd, 0);
}

void
engine_run_uci_cmd(struct Engine *engine) {
	char *arg_0 = uci_cmd_iter_next(engine->cmd_iter);
	if (strcmp(arg_0, "uci") == 0) {
		engine_uci_uci(engine);
	} else if (strcmp(arg_0, "debug") == 0) {
		engine_uci_debug(engine);
	} else if (strcmp(arg_0, "newgame") == 0) {
		engine_uci_new_game(engine);
	} else if (strcmp(arg_0, "position") == 0) {
		engine_uci_position(engine);
	} else if (strcmp(arg_0, "go") == 0) {
		engine_uci_go(engine);
	} else if (strcmp(arg_0, "isready") == 0) {
		engine_uci_is_ready(engine);
	} else if (strcmp(arg_0, "stop") == 0) {
		engine_uci_stop(engine);
	} else if (strcmp(arg_0, "quit") == 0) {
		engine_uci_quit(engine);
	}
}

uint8_t
engine_wait_until_quitting(struct Engine *engine) {
	while (engine->status != STATUS_QUIT) {
		engine_wait_for_uci_cmd(engine);
		engine_run_uci_cmd(engine);
	}
	return EX_OK;
}

void
engine_uci_uci(struct Engine *engine) {
	if (engine->status != STATUS_BOOT) {
		return;
	}
	printf("id name %s\n", ENGINE_NAME);
	printf("id author %s\n", ENGINE_AUTHOR);
	engine->status = STATUS_WAIT;
	printf("uciok\n");
}

void
engine_uci_debug(struct Engine *engine) {
	char *arg_1 = uci_cmd_iter_next(engine->cmd_iter);
	if (!arg_1 || strcmp(arg_1, "true") == 1) {
		engine->debug = true;
	} else if (strcmp(arg_1, "false") == 1) {
		engine->debug = false;
	}
}

void
engine_uci_is_ready(struct Engine *engine) {
	printf("readyok\n");
}

void
engine_uci_set_option(struct Engine *engine) {
	char *arg;
	char *name = NULL;
	char *value = NULL;
	if (strcmp(uci_cmd_iter_next(engine->cmd_iter), "name") != 0) {
		return;
	}
	while ((arg = uci_cmd_iter_next(engine->cmd_iter)) && strcmp(arg, "value") == 1) {
		// TODO
	}
}

void
engine_uci_register(struct Engine *engine) {
	return;
}

void
engine_uci_new_game(struct Engine *engine) {
	return;
}

void
engine_uci_position(struct Engine *engine) {
	char *arg = uci_cmd_iter_next(engine->cmd_iter);
	if (strcmp(arg, "startpos") == 0) {
		board_new(engine->board);
	} else if (strcmp(arg, "fen") == 0 && engine->cmd->argc >= 8) {
		fen_specify_pieces(uci_cmd_iter_next(engine->cmd_iter), engine->board);
		fen_specify_active_player(uci_cmd_iter_next(engine->cmd_iter), engine->board);
		fen_specify_castling_rights(uci_cmd_iter_next(engine->cmd_iter), engine->board);
		fen_specify_en_passant_target(uci_cmd_iter_next(engine->cmd_iter), engine->board);
		fen_specify_half_moves(uci_cmd_iter_next(engine->cmd_iter), engine->board);
		fen_specify_full_moves(uci_cmd_iter_next(engine->cmd_iter), engine->board);
	}
	while ((arg = uci_cmd_iter_next(engine->cmd_iter))) {
		// TODO: evaluate additional moves.
		break;
	}
}

void
engine_uci_go(struct Engine *engine) {
	char *arg;
	// TODO/FIXME
	while ((arg = uci_cmd_iter_next(engine->cmd_iter))) {
		if (strcmp(arg, "ponder") == 0) {
			engine->params->ponder = true;
		} else if (strcmp(arg, "wtime") == 0) {
			engine->params->white_time = atoi(uci_cmd_iter_next(engine->cmd_iter));
		} else if (strcmp(arg, "btime") == 0) {
			engine->params->black_time = atoi(uci_cmd_iter_next(engine->cmd_iter));
		} else if (strcmp(arg, "winc") == 0) {
			engine->params->white_increment = atoi(uci_cmd_iter_next(engine->cmd_iter));
		} else if (strcmp(arg, "binc") == 0) {
			engine->params->black_increment = atoi(uci_cmd_iter_next(engine->cmd_iter));
		} else if (strcmp(arg, "movestogo") == 0) {
			engine->params->moves_to_go = atoi(uci_cmd_iter_next(engine->cmd_iter));
		} else if (strcmp(arg, "depth") == 0) {
			engine->params->depth = atoi(uci_cmd_iter_next(engine->cmd_iter));
		} else if (strcmp(arg, "nodes") == 0) {
			engine->params->num_nodes = atoi(uci_cmd_iter_next(engine->cmd_iter));
		} else if (strcmp(arg, "mate") == 0) {
			engine->params->mate_length = atoi(uci_cmd_iter_next(engine->cmd_iter));
		} else if (strcmp(arg, "movetime") == 0) {
		} else if (strcmp(arg, "infinite") == 0) {
			engine->params->infinite = true;
		}
	}
}

void
engine_uci_stop(struct Engine *engine) {
	return;
}

void
engine_uci_ponder_hit(struct Engine *engine) {
	return;
}

void
engine_uci_quit(struct Engine *engine) {
	engine->status = STATUS_QUIT;
}
