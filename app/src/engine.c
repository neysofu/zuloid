/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "engine.h"
#include "chess/board.h"
#include "chess/fen.h"
#include "chess/move.h"
#include "chess/result.h"
#include "clock.h"
#include "debug.h"
#include "globals.h"
#include "mode.h"
#include "mt19937-64/mt64.h"
#include "search/ttable.h"
#include "settings.h"
#include "switches.h"
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
	engine->board = BOARD_STARTPOS;
	engine->result = RESULT_NONE;
	engine->mode = MODE_IDLE;
	engine->exit_status = EX_OK;
	settings_default(&engine->settings);
	engine->ttable = NULL;
	return engine;
}

void
engine_free(struct Engine *engine)
{
	assert(engine);
	free(engine);
}

int8_t
engine_main(struct Engine *engine)
{
	assert(engine);
	DEBUG("Entered UCI loop.");
	char *cmd = NULL;
	char *cmd_iter = NULL;
	size_t cmd_length = 0;
	while (engine->mode != MODE_EXIT) {
		if (getline(&cmd, &cmd_length, stdin) == -1) {
			DEBUG("Something unexpected happened while reading from stdin. Abort.");
			engine->mode = MODE_EXIT;
			engine->exit_status = EX_IOERR;
			return engine->exit_status;
		}
		cmd_iter = cmd;
		while (isspace(*cmd_iter)) {
			cmd_iter++;
		}
		if (!*cmd_iter || *cmd_iter == '#') {
			continue;
		}
		const char *response = engine_rpc(engine, cmd);
		if (response) {
			DEBUG("Writing the JSON RPC response object to stdout.");
			printf("\t%s\r\n", response);
			free((void *)(response));
		}
	}
	DEBUG("Goodbye.");
	return engine->exit_status;
}
