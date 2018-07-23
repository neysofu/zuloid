#pragma once

#include <stdbool.h>
#include "termination.h"

#define ENGINE_NAME "Z64C v0.4"
#define ENGINE_AUTHOR "Filippo Costa (@neysofu)"
#define TRAINING false

struct Engine;

struct Engine *
engine_new(struct Engine *engine);

void
engine_drop(struct Engine *engine);

int
engine_main(struct Engine *engine);

void
engine_call(struct Engine *engine, char *cmd);

enum Termination
engine_termination(struct Engine *engine);
