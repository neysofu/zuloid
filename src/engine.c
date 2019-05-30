/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "engine.h"
#include "cache/cache.h"
#include "chess/position.h"
#include "core/agent.h"
#include "protocols.h"
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

struct Engine *
engine_new(void)
{
	struct Engine *engine = malloc(sizeof(struct Engine));
	if (engine) {
		*engine = (struct Engine){
			.position = POSITION_INIT,
			.time_controls = { NULL, NULL },
			.cache = NULL,
			.agent = NULL,
			.port = 34290,
			.seed = 0xcfca130b,
			.debug = false,
			.move_selection_noise = 0.005,
			.contempt = 0.65,
			.selectivity = 0.5,
			.protocol = PROTOCOL_UNKNOWN,
			.mode = MODE_IDLE,
			.exit_status = EXIT_SUCCESS,
		};
	}
	return engine;
}

int
engine_delete(struct Engine *engine)
{
	if (!engine) {
		return EXIT_SUCCESS;
	}
	int exit_status = engine->exit_status;
	time_control_delete(engine->time_controls[COLOR_WHITE]);
	time_control_delete(engine->time_controls[COLOR_BLACK]);
	cache_delete(engine->cache);
	agent_delete(engine->agent);
	free(engine);
	return exit_status;
}

void
engine_call(struct Engine *engine, char *cmd)
{
	assert(engine);
	assert(cmd);
	switch (engine->protocol) {
		case PROTOCOL_UNKNOWN:
			engine_unknown_protocol(engine, cmd);
			break;
		case PROTOCOL_UCI:
			engine_uci(engine, cmd);
			break;
		default:
			printf("This protocol is not (yet?) supported. Aborting.");
			engine->mode = MODE_EXIT;
	}
}

void
engine_logf(struct Engine *engine,
            const char *filename,
            const char *function_name,
            size_t line_num,
            ...)
{
	assert(engine);
	assert(filename);
	assert(function_name);
#ifndef NDEBUG
	va_list args;
	va_start(args, line_num);
	printf("# %s:%s:%zu -- ", filename + PROJECT_DIR_LENGTH, function_name, line_num);
	vprintf(va_arg(args, const char *), args);
	va_end(args);
#endif
}
