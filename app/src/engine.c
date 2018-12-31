/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "engine.h"
#include "chess/position.h"
#include "search/cache.h"
#include "settings.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

struct Engine *
engine_new(void)
{
	struct Engine *engine = handle_oom(malloc(sizeof(struct Engine)));
	engine->position = POSITION_DEFAULT;
	engine->winner = COLOR_NONE;
	engine->termination = TERMINATION_NONE;
	engine->game_clocks[COLOR_WHITE] = NULL;
	engine->game_clocks[COLOR_BLACK] = NULL;
	engine->settings = DEFAULT_SETTINGS;
	engine->cache = NULL;
	engine->notifications_stream = stdout;
	engine->mode = MODE_IDLE;
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

int
engine_main(struct Engine *engine)
{
	print_welcome_message();
	while (engine->mode != MODE_EXIT) {
		char *str = read_line_from_stream(stdin);
		if (!str) {
			printf("{\"jsonrpc\":\"2.0\",\"id\":null,\"error\":{\"code\":0,\"message\":"
			       "\"Input error\"}}\r\n");
			return EXIT_FAILURE;
		}
		char *response = engine_call(engine, str);
		if (response) {
			/* A tab character before the response string visually separates requests from
			 * responses in the terminal. */
			printf("\t%s\r\n", response);
			free(response);
		}
	}
	return EXIT_SUCCESS;
}
