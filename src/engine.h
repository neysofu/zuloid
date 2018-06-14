#pragma once

#include <stdbool.h>
#include "board.h"
#include "move.h"

#define ENGINE_NAME "Z64C v0.1"
#define ENGINE_AUTHOR "Filippo Costa (@neysofu)"
#define TRAINING false

struct Engine;

struct Engine *
engine_new(struct Engine *engine);

void
engine_drop(struct Engine *engine);

uint8_t
engine_main(struct Engine *engine);

void
engine_call(struct Engine *engine, char *cmd);


