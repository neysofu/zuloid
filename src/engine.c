/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "engine.h"
#include "chess/position.h"
#include "engine.h"
#include "search/cache.h"
#include "settings.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct Engine *
engine_new(void)
{
	struct Engine *engine = malloc_or_exit(sizeof(struct Engine));
	*engine = (struct Engine){
		.position = POSITION_DEFAULT,
		.winner = COLOR_NONE,
		.termination = TERMINATION_NONE,
		.game_clocks = { NULL, NULL },
		.settings = SETTINGS_DEFAULT,
		.cache = NULL,
		.notifications_stream = stdout,
		.mode = MODE_IDLE,
		.exit_status = EXIT_SUCCESS,
	};
	return engine;
}

void
engine_delete(struct Engine *engine)
{
	if (!engine) {
		return;
	}
	game_clock_delete(engine->game_clocks[COLOR_WHITE]);
	game_clock_delete(engine->game_clocks[COLOR_BLACK]);
	cache_delete(engine->cache);
	free(engine);
}

const int *
engine_exit_status(const struct Engine *engine)
{
	if (engine->mode == MODE_EXIT) {
		return &engine->exit_status;
	} else {
		return NULL;
	}
}
