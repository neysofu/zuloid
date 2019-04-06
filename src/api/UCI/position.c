/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chess/position.h"
#include "chess/fen.h"
#include "engine.h"
#include "utils/dyn_str.h"
#include <assert.h>
#include <string.h>

void
engine_uci_call_position(struct Engine *engine, struct DynStr *dyn_str)
{
	assert(engine);
	assert(dyn_str);
	if (strcmp(dyn_str_next_token(dyn_str), "startpos") == 0) {
		engine->position = POSITION_INIT;
	} else if (strcmp(dyn_str_current_token(dyn_str), "fen") == 0) {
		position_init_from_dyn_str(&engine->position, dyn_str);
	}
	while (dyn_str_next_token(dyn_str)) {
		// TODO
		//	position_push(string_to_move(cmd_current(&cmd)));
	}
}
