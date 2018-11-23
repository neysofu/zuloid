/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * -------------------------------------------------------------------
 *
 * The `Engine` structure holds a reference to all engine components and wires
 * them together. */

#pragma once

#include "chess/board.h"
#include "chess/legality.h"
#include "chess/result.h"
#include "chess/termination.h"
#include "game.h"
#include "mode.h"
#include "search/ttable.h"
#include "settings.h"
#include <stdbool.h>

/* A self-contained chess engine instance.
 * The `Engine` structure contains all the necessary components to make Z64C
 * work.
 * This structure doesn't takes up very little memory by itself. The most
 * resource-hungry component by far and large is 'struct TTable'. */
struct Engine
{
	/* The current chess position.
	 */
	struct Board board;
	/* A flag that signals when somebody wins. */
	struct Result result;
	/* Indexed by `enum Color`. */
	struct Clock *time_control[2];
	struct Settings settings;
	/* What to do next? `mode` tells us. */
	enum Mode mode;
	/* On exit, this variable stores the exit status returned by the process. */
	int8_t exit_status;
	struct TTable *ttable;
	struct Network *network;
};

/* Create a self-contained engine instance with default settings. */
struct Engine *
engine_new(void);

/* Gracefully kill a `Engine` instance. */
void
engine_free(struct Engine *engine);

/* Start reading commands from stdin and feen them to `engine` as they come. */
int8_t
engine_main(struct Engine *engine);

/* Runs a "Remote Procedure Call" (RPC) and return the engine's response.
 * All communication must be valid JSON-RPC 2.0.
 *
 * Parameters
 *   `cmd` is a valid pointer to a null-terminated string that encodes JSON-RPC.
 * Result
 *   It returns a pointer to a null-terminated serialized JSON-RPC response
 *   object. The JSON data contains no unnecessary whitespace and doesn't
 *   contain any newline character.
 * Preconditions
 *   `cmd` must be a valid pointer to a null-terminated string. */
const char *
engine_rpc(struct Engine *engine, const char *cmd);
