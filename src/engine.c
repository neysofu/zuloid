#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sysexits.h>
#include <sys/select.h>
#include "xxhash.h"
#include "engine.h"
#include "util.h"
#include "fen.h"
#include "chessboard.h"

#define UCI_XXH64_UCI 0xf80028c1113b2c9c
#define UCI_XXH64_DEBUG 0x01fd51a2a6f9cc2f
#define UCI_XXH64_NEW_GAME 0xdfd89a3bb7b15ce5
#define UCI_XXH64_IS_READY 0x33e37def10e5d195
#define UCI_XXH64_POSITION 0x8a7ecc4137c6f2b0
#define UCI_XXH64_GO 0xd682e29388e0f4a3
#define UCI_XXH64_GO_SEARCHMOVES 0xbefa9b18e172da9a
#define UCI_XXH64_GO_PONDER 0x0a6f394a3987568a
#define UCI_XXH64_GO_WTIME 0x2a8ef3657cf9a920
#define UCI_XXH64_GO_BTIME 0xd3f6a6885c7c93a0
#define UCI_XXH64_GO_WINC 0x71c2388517319e0c
#define UCI_XXH64_GO_BINC 0xad513987341315ae
#define UCI_XXH64_GO_MOVESTOGO 0xf95e9c242b5d7a1a
#define UCI_XXH64_STOP 0xd8d95334d91f61fc
#define UCI_XXH64_PONDER_HIT 0xe13f0de8ec4aa474
#define UCI_XXH64_QUIT 0x707db5f765aed6d8
#define UCI_XXH64_SET_OPTION 0x26cc87cdbb3247ba
#define UCI_XXH64_REGISTER 0x6f84e39db7f2df07

enum Status {
	STATUS_BOOT,
	STATUS_WAIT,
	STATUS_QUIT,
};

struct Engine {
	struct UciCmd *cmd;
	struct Chessboard *cb;
	bool is_searching;
	bool debug;
	enum Status status;
	struct TranspositionsTable *t_table;
	struct SearchParams *params;
};

struct Engine *
engine_new(void) {
	engine = malloc(sizeof(struct Engine) + sizeof(struct Chessboard));
	engine->cmd = uci_cmd_new();
	if (!engine || !(engine->cmd)) { return EX_OSERR; }
	engine->cb = engine + sizeof(struct Chessboard);
	engine->is_searching = false;
	*(engine->cb) = CB_INIT;
	engine->debug = false;
	engine->status = STATUS_BOOT;
	return EX_OK;
}

void
engine_drop(struct Engine *engine) {
	uci_cmd_drop(engine->cmd);
	free(engine->cb);
	free(engine);
}

void
engine_wait_for_command(struct Engine *engine) {
	engine->cmd = uci_cmd_from_str(engine->cmd);
	if (!(engine->cmd) || engine->cmd->argc == 0) {
		return;
	}
	switch ((uint64_t)(XXH64(uci_cmd_arg(engine->arg, 0), strlen(engine->cmd), 0))) {
		case UCI_XXH64_UCI:
			engine_uci_uci(engine);
			break;
		case UCI_XXH64_DEBUG:
			if (engine->cmd->argc >= 2) {
				bool debug = uci_cmd_arg(engine->cmd, 1);
			} else {
				bool debug = false;
			}
			engine_uci_debug(engine, debug);
			break;
		case UCI_XXH64_NEW_GAME:
			engine_uci_new_game(engine);
			break;
		case UCI_XXH64_POSITION:
			if (engine->cmd_argc >= 2) {
				if (engine_arg(engine, 1) == "fen") {
					engine->cb = CB_INIT;
				} else {
					cb_specify_pieces
				}
			}
			engine_uci_position(engine);
			break;
		case UCI_XXH64_GO:
			struct SearchParams *params = search_params_from_uci_cmd(engine->cmd);
			engine_uci_go(engine, params);
			break;
		case UCI_XXH64_IS_READY:
			engine_uci_is_ready(engine);
			break;
		case UCI_XXH64_STOP:
			engine_uci_stop(engine);
			break;
		case UCI_XXH64_QUIT:
			engine_uci_quit(engine);
			break;
		default:
			return;
	}
}

uint8_t
engine_wait_until_quitting(struct Engine *engine) {
	while (engine->status != STATUS_QUIT) {
		engine_wait_for_command();
	}
	return EX_OK;
}

char *
engine_cmd_arg(size_t i) {
	if (i >= engine->cmd_argc) { return NULL; }
	char *arg = engine->cmd;
	while ((i--) > 0) {
		// This skips the argument's null-terminator.
		arg += strlen(arg) + 1;
	}
	return arg;
}
