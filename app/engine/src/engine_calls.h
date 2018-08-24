#pragma once

#include "engine.h"

// The following functions will all parse 'engine->cmd' from the second token
// onwards and evaluate the command accordingly to the UCI instruction they were
// named after.

void
engine_call_debug(struct Engine *engine);

void
engine_call_go(struct Engine *engine);

void
engine_call_help(struct Engine *engine);

void
engine_call_is_legal(struct Engine *engine);

void
engine_call_move(struct Engine *engine);

void
engine_call_new_game(struct Engine *engine);

void
engine_call_ponder_hit(struct Engine *engine);

void
engine_call_position(struct Engine *engine);

void
engine_call_set_option(struct Engine *engine);

void
engine_call_stop(struct Engine *engine);

void
engine_call_uci(struct Engine *engine);
