#pragma once

#include "chessboard.h"
#include "move.h"

#define ENGINE_NAME "Z64C"
#define ENGINE_AUTHOR "Filippo Costa (@neysofu)"

struct Engine;

struct Engine *
engine_new(void);

void
engine_drop(struct Engine *);

uint8_t
engine_wait_until_quitting(struct Engine *engine);
