#pragma once

#include "engine.h"

// The following functions will all parse 'engine->cmd' from the second token
// onwards and evaluate the command accordingly to the UCI instruction they were
// named after.

void
engine_call_help(struct Engine *engine);

void
engine_call_move(struct Engine *engine);

void
engine_call_set(struct Engine *engine);

void
engine_call_setup(struct Engine *engine);

void
engine_call_think(struct Engine *engine);
