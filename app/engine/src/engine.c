/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "engine.h"
#include "chess/board.h"
#include "chess/fen.h"
#include "chess/move.h"
#include "chess/result.h"
#include "clock.h"
#include "globals.h"
#include "mode.h"
#include "mt19937-64/mt64.h"
#include "search/ttable.h"
#include "settings.h"
#include "switches.h"
#include "trace.h"
#include "utils.h"
#include "xxHash.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sysexits.h>
#include <unistd.h>

struct Engine *
engine_new(void)
{
	struct Engine *engine = xmalloc(sizeof(struct Engine));
	engine->ttable = ttable_new();
	engine->scontroller = scontroller_new(engine->ttable);
	engine->board = BOARD_STARTPOS;
	engine->result = RESULT_NONE;
	engine->mode = MODE_IDLE;
	engine->exit_status = EX_OK;
	settings_default(&engine->settings);
	return engine;
}

void
engine_free(struct Engine *engine)
{
	TRACE("Freeing all memory.\n");
	assert(engine);
	ttable_free(engine->ttable);
	scontroller_free(engine->scontroller);
	free(engine);
}

int8_t
engine_main(struct Engine *engine)
{
	TRACE("Now entering the main UCI loop.\n");
	assert(engine);
	char *cmd = NULL;
	size_t cmd_length = 0;
	while (engine->mode != MODE_EXIT) {
		if (getline(&cmd, &cmd_length, stdin) == -1) {
			TRACE("Something bad happened while reading from stdin. Abort.\n");
			return EX_IOERR;
		}
		char cmd_iter = cmd;
		if (util_str_is_whitespace(cmd)) {
			continue;
		}
		const char *response = engine_rpc(engine, cmd);
		if (response) {
			TRACE("Writing the JSON RPC response object to stdout.\n");
			printf("  %s\r\n", response);
			free((void *)(response));
		}
	}
	TRACE("Goodbye.\n");
	return engine->exit_status;
}
