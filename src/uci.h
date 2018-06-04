#pragma once

#include "engine.h"
#include "board.h"
#include "search.h"
#include "move.h"

struct UciCmd;

struct UciCmd *
uci_cmd_new(void);

void
uci_cmd_drop(struct UciCmd *cmd);

struct UciCmd *
str_to_uci_cmd(struct UciCmd *cmd, char *str, size_t size);

void
uci_cmd_add_arg(struct UciCmd *cmd, char *arg, size_t size);

void
uci_cmd_arg_merge(struct UciCmd *cmd, size_t from, size_t to);

void
engine_uci_uci(struct Engine *engine);

void
engine_uci_debug(struct Engine *engine, bool debug);

void
engine_uci_is_ready(struct Engine *engine);

void
engine_uci_set_option(struct Engine *engine, char *name, char *value);

enum RegistrationMode {
	REGISTRATION_MODE_LATER,
	REGISTRATION_MODE_NAME,
	REGISTRATION_MODE_CODE,
};

void
engine_uci_register(struct Engine *engine, enum RegistationMode mode);

void
engine_uci_new_game(struct Engine *engine);

void
engine_uci_position(struct Engine *engine, struct Chessboard *cb, struct Move[] moves);

void
engine_uci_go(struct Engine *engine, struct SearchParams *params);

void
engine_uci_stop(struct Engine *engine);

void
engine_uci_ponder_hit(struct Engine *engine);

void
engine_uci_quit(struct Engine *engine);
