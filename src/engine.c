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

#define UCI_HASH_UCI 0x2c572e67c6
#define UCI_HASH_DEBUG 0xbc4ae6f131d8b22c
#define UCI_HASH_NEW_GAME 0x428ec30e91e1a9e9
#define UCI_HASH_IS_READY 0xc87bfcb18021d096
#define UCI_HASH_POSITION 0xe983bba8402669ba
#define UCI_HASH_POSITION_FEN 0x20347424d57c4f3e
#define UCI_HASH_POSITION_STARTPOS 0x68c717dc305fff25
#define UCI_HASH_GO 0xebd0ee6475b1d0fb
#define UCI_HASH_GO_SEARCHMOVES 0x6d42f01660871b25
#define UCI_HASH_GO_PONDER 0xd7315bd44c01f5ad
#define UCI_HASH_GO_WTIME 0xf7a3c003bc59c62b
#define UCI_HASH_GO_BTIME 0xf7a3c003baddc396
#define UCI_HASH_GO_WINC 0x7da1287dbd2fa1d6
#define UCI_HASH_GO_BINC 0x7da1287dbd241de1
#define UCI_HASH_GO_MOVESTOGO 0xfbd1a5a5255d21e8
#define UCI_HASH_STOP 0xb7b5715ee2e9b84b
#define UCI_HASH_PONDER_HIT 0xd3691fa06f58bff2
#define UCI_HASH_QUIT 0x5ddd2e9f271a0208
#define UCI_HASH_SET_OPTION 0x6a9871f352cdadea
#define UCI_HASH_REGISTER 0xb179be25c5193b8a

enum Status {
	STATUS_BOOT,
	STATUS_WAIT,
	STATUS_QUIT,
};

struct Engine {
	struct UciCmd *cmd;
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
	uci_cmd_drop(engine->cmd);
	board_drop(engine->board);
	free(engine);
}

void
engine_wait_for_uci_cmd(struct Engine *engine) {
	size_t line_len = 0;
	if ((line_len = getline(&(engine->cmd->str), &line_len, stdin)) == 1) {
		return;
	}
	engine->cmd = str_to_uci_cmd(engine->cmd->str, line_len, engine->cmd);
}

void
engine_run_uci_cmd(struct Engine *engine) {
	if (engine->cmd->argc == 0) {
		return;
	}
	char *arg_0 = uci_cmd_arg(engine->cmd, 0);
	size_t arg_0_len = uci_cmd_arg_len(engine->cmd, 0);
	switch (hash(arg_0, arg_0_len)) {
		case UCI_HASH_UCI:
			engine_uci_uci(engine);
			break;
		case UCI_HASH_DEBUG:
			engine_uci_debug(engine);
			break;
		case UCI_HASH_NEW_GAME:
			engine_uci_new_game(engine);
			break;
		case UCI_HASH_POSITION:
			engine_uci_position(engine);
			break;
		case UCI_HASH_GO:
			engine_uci_go(engine);
			break;
		case UCI_HASH_IS_READY:
			engine_uci_is_ready(engine);
			break;
		case UCI_HASH_STOP:
			engine_uci_stop(engine);
			break;
		case UCI_HASH_QUIT:
			engine_uci_quit(engine);
			break;
		default:
			return;
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
	bool debug = (engine->cmd->argc >= 2) ? (false /* TODO */) : true;
	engine->debug = debug;
}

void
engine_uci_is_ready(struct Engine *engine) {
	printf("readyok\n");
}

void
engine_uci_set_option(struct Engine *engine) {
	return;
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
	if (engine->cmd->argc < 2) {
		return;
	}
	char *init_mode = uci_cmd_arg(engine->cmd, 1);
	size_t init_mode_len = uci_cmd_arg_len(engine->cmd, 1);
	switch (hash(init_mode, init_mode_len)) {
		case UCI_HASH_POSITION_STARTPOS:
			board_new(engine->board);
			break;
		case UCI_HASH_POSITION_FEN:
			if (engine->cmd->argc < 8) {
				return;
			}
			struct UciCmdIter *iter = uci_cmd_iter_new(NULL, engine->cmd, 2);
			fen_specify_pieces(uci_cmd_iter_next(iter), engine->board);
			fen_specify_active_player(uci_cmd_iter_next(iter), engine->board);
			fen_specify_castling_rights(uci_cmd_iter_next(iter), engine->board);
			fen_specify_en_passant_target(uci_cmd_iter_next(iter), engine->board);
			fen_specify_half_moves(uci_cmd_iter_next(iter), engine->board);
			fen_specify_full_moves(uci_cmd_iter_next(iter), engine->board);
			uci_cmd_iter_drop(iter);
	}
	// TODO: evaluate additional moves.
}

void
engine_uci_go(struct Engine *engine) {
	struct SearchParams *params = uci_cmd_to_search_params(engine->cmd, NULL);
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
