/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * The `Engine` structure holds a reference to all engine components and wires
 * them together. */

#ifndef Z64C_ENGINE_H
#define Z64C_ENGINE_H

#include "chess/position.h"
#include "chess/termination.h"
#include "eval/eval_function.h"
#include "search/cache.h"
#include "search/search_results.h"
#include "settings.h"
#include "time/game_clock.h"
#include <stdio.h>

enum Mode
{
	/* The engine is sitting idle and waiting for commands. */
	MODE_IDLE,
	/* The engine is currently busy searching. */
	MODE_SEARCH,
	/* The engine is ready to exit. */
	MODE_EXIT,
};

struct Engine
{
	struct Position position;
	Color winner;
	enum Termination termination;
	/* Indexed by `enum Color`. By default both sides have infinite time to think. */
	struct GameClock *game_clocks[2];
	struct Settings settings;
	struct Cache *cache;
	struct EvalFunction *eval_function;
	struct SearchResults search_results;
	FILE *notifications_stream;
	enum Mode mode;
	int exit_status;
};

/* Creates a self-contained engine instance with default settings.
 *
 * Result
 *  NULL on failure; a valid pointer on success. */
struct Engine *
engine_new(void);

/* Gracefully kills `engine`. No shutdown JSON-RPC Request is needed. */
void
engine_delete(struct Engine *engine);

/* Runs a "Remote Procedure Call" (RPC) and returns the engine's response.
 * All communication must be in valid JSON-RPC 2.0.
 *
 * Result
 *   A JSON-RPC response string without line breaks. */
char *
engine_call(struct Engine *engine, const char *request);

#endif
