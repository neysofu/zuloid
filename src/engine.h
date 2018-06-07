#pragma once

#include "board.h"
#include "move.h"

#define ENGINE_NAME "Z64C v0.1"
#define ENGINE_AUTHOR "Filippo Costa (@neysofu)"

struct Engine;

struct Engine *
engine_new(struct Engine *engine);

void
engine_drop(struct Engine *engine);

uint8_t
engine_wait_until_quitting(struct Engine *engine);

void
engine_wait_for_uci_cmd(struct Engine *engine);

void
engine_run_uci_cmd(struct Engine *engine);

void
engine_uci_uci(struct Engine *engine);

void
engine_uci_debug(struct Engine *engine);

void
engine_uci_is_ready(struct Engine *engine);

void
engine_uci_set_option(struct Engine *engine);

void
engine_uci_register(struct Engine *engine);

void
engine_uci_new_game(struct Engine *engine);

void
engine_uci_position(struct Engine *engine);

void
engine_uci_go(struct Engine *engine);

void
engine_uci_stop(struct Engine *engine);

void
engine_uci_ponder_hit(struct Engine *engine);

void
engine_uci_quit(struct Engine *engine);
