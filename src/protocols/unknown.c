/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "engine.h"
#include <assert.h>
#include <string.h>

void
engine_unknown_protocol(struct Engine *engine, char *cmd)
{
	assert(engine);
	assert(cmd);
	if (strstr(cmd, "method")) {
		engine->protocol = PROTOCOL_UGEI;
	} else if (strstr(cmd, "uci")) {
		engine->protocol = PROTOCOL_UCI;
	} else if (strcmp(cmd, "xboard")) {
		engine->protocol = PROTOCOL_CECP;
	}
	engine_call(engine, cmd);
}
