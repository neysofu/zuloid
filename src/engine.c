/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "engine.h"
#include "cache/cache.h"
#include "chess/position.h"
#include "core/agent.h"
#include "protocols.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void
engine_new(struct Engine *engine)
{
	assert(engine);
	*engine = (struct Engine){
		.position = POSITION_DEFAULT,
		.time_controls = { NULL, NULL },
		.cache = NULL,
		.agent = NULL,
		.mode = MODE_IDLE,
		.protocol = engine_unknown_protocol,
		.port = 34290,
		.seed = 0xcfca130bUL,
		.move_selection_noise = 0.005,
		.contempt = 0.65,
		.selectivity = 0.5,
		.exit_status = EXIT_SUCCESS,
	};
}

void
engine_delete(struct Engine *engine)
{
	if (!engine) {
		return;
	}
	time_control_delete(engine->time_controls[COLOR_WHITE]);
	time_control_delete(engine->time_controls[COLOR_BLACK]);
	cache_delete(engine->cache);
	agent_delete(engine->agent);
}

void
engine_call(struct Engine *engine, char *string)
{
	assert(engine);
	assert(string);
	assert(engine->mode != MODE_EXIT);
	(*engine->protocol)(engine, string);
}
