/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_ENGINE_H
#define Z64C_ENGINE_H

#include "cache/cache.h"
#include "chess/position.h"
#include "chess/termination.h"
#include "core/agent.h"
#include "core/eval.h"
#include "time/game_clock.h"
#include <stdio.h>

#define ENGINE_LOGF(engine, ...)                                                           \
	engine_logf(engine, __FILE__, __func__, __LINE__, __VA_ARGS__)

enum Protocol
{
	PROTOCOL_CECP,
	PROTOCOL_UCI,
	PROTOCOL_UNKNOWN,
};

enum Mode
{
	/* No background activity. The engine is just waiting for user input. */
	MODE_IDLE,
	/* While still accepting user input, the engine is searching the game tree
	 * in the background. */
	MODE_SEARCH,
	/* The engine will not accept other user input. This mode is not
	   recoverable. */
	MODE_EXIT,
};

struct Engine
{
	/* Only one position at the time. */
	struct Position position;
	/* Indexed by `enum Color`. By default both sides have infinite time to
	 * think. */
	struct TimeControl *time_controls[2];
	struct GameClock game_clocks[2];
	struct Cache *cache;
	struct Agent *agent;
	struct Eval eval;
	struct Tablebase *tablebase;
	/* All network activity (if any) will be on this port. */
	int port;
	int32_t seed;
	bool verbose;
	bool ponder;
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
	/* Communication protocol used to talk to the chess GUI. */
	enum Protocol protocol;
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

/* A protocol-agnostic entry point for engine scripting.
 *
 * @param engine The engine instance that must process the command string.
 * @param string The command string.
 * @sideeffect Possibly, a response will be printed to stdout. `engine`'s
 * internal data also can change. */
void
engine_call(struct Engine *engine, char *cmd);

/* Spawns a new thread and searches the current position. That thread will be killed when
 * (if ever) the search yields a result. */
void
engine_start_search(struct Engine *engine);
void
engine_stop_search(struct Engine *engine);

void
engine_logf(struct Engine *engine,
            const char *filename,
            const char *function_name,
            size_t line_num,
            ...);

/* Gracefully kills `engine` and all its subsystems. Despite what methods such
 * as UCI's "quit" can induce into thinking, they are completely optional. No
 * previous action is necessary before calling this function.
 *
 * @param engine the non-NULL memory location to clean up.
 * @sideeffect All pointers stored inside `engine` are set to NULL and
 * their memory locations become unaccessible. */
int
engine_delete(struct Engine *engine);

#endif
