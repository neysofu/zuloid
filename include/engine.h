/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * The `Engine` structure holds a reference to all engine components and wires
 * them together. */

#ifndef Z64C_ENGINE_H
#define Z64C_ENGINE_H

#include "cache/cache.h"
#include "chess/position.h"
#include "chess/termination.h"
#include "core/agent.h"
#include "core/eval.h"
#include "time/game_clock.h"
#include "utils/dyn_str.h"
#include <stdio.h>

enum Mode
{
	/* No background activity. The engine is just waiting for user input. */
	MODE_IDLE,
	/* While still accepting user input, the engine is searching the game tree
	 * in the background. */
	MODE_SEARCH,
	/* The engine will not accept other user input. This mode is not recoverable. */
	MODE_EXIT,
};

struct Engine
{
	/* Only one position at the time. */
	struct Position position;
	/* Indexed by `enum Color`. By default both sides have infinite time to think. */
	struct TimeControl *time_controls[2];
	struct GameClock game_clocks[2];
	struct Cache *cache;
	struct Agent *agent;
	struct Eval eval;
	/* It points to `engine_call`'s correct implementation after protocol setting. */
	void (*protocol)(struct Engine *, struct DynStr *);
	struct Tablebase *tablebase;
	/* All network activity (if any) will be on this port. */
	int port;
	int seed;
	bool debug;
	float move_selection_noise;
	/* Must be in the range [0,1]. It measures the engine's sense of
	 * superiority and thus reluctancy to draw. When set to 0, draws are
	 * effectively treated as wins; when set to 1, as losses. Tune it according
	 * the opponent's rating for best performance. */
	float contempt;
	/* Must be in the range [0,1]. The greater its value, the more selective
	 * the engine will be in searching the game tree. Warning! Even little
	 * adjustments can have extensive influence over the gameplay. 0.5 is the
	 * most performant option. */
	float selectivity;
	/* A straightforward activity indicator. Both `main` and engine commands
	 * might want to know if the engine is doing background computation or
	 * what. */
	enum Mode mode;
	/* Can be set together with `mode = MODE_EXIT` to exit the program. */
	int exit_status;
};

/* Initializes a self-contained engine instance at the address 'engine'.
 *
 * Please note that initialization must be completed via the 'init' command.
 * @param engine the non-NULL memory location to initialize.
 *
 * RETURN VALUE: NULL on failure; a valid pointer on success. */
struct Engine *
engine_new(void);

/* Gracefully kills `engine` and all its subsystems. Despite what methods such
 * as UCI's "quit" can induce into thinking, they are completely optional. No
 * previous action is necessary before calling this function.
 *
 * @param engine the non-NULL memory location to clean up.
 * @sideeffect All pointers stored inside `engine` are set to NULL and
 * their memory locations become unaccessible. */
int
engine_delete(struct Engine *engine);

/* A protocol-agnostic entry point for engine scripting.
 *
 * @param engine The engine instance that must process the command string.
 * @param string The command string.
 * @sideeffect Possibly, a response will be printed to stdout. `engine`'s
 * internal data also can change.
 *
 * Runs a "Remote Procedure Call" (RPC) and returns the engine's response.
 * All communication must be in valid JSON-RPC 2.0. */
void
engine_call(struct Engine *engine, struct DynStr *dyn_str);

#endif
