/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chess/fen.h"
#include "chess/position.h"
#include "engine.h"
#include "utils/dyn_str.h"
#include "xxHash/xxhash.h"
#include <assert.h>
#include <string.h>

void
engine_uci_call_go(struct Engine *engine, struct DynStr *dyn_str)
{
	assert(engine);
	assert(dyn_str);
	char *token = dyn_str_next_token(dyn_str);
	switch (XXH64(token, strlen(token), 0)) {
		case 0x2a8ef3657cf9a920: /* "wtime" */
			token = dyn_str_next_token(dyn_str);
			engine->time_controls[COLOR_WHITE]->time_limit_in_seconds = atoi(token) * 1000;
			break;
		case 0xd3f6a6885c7c93a0: /* "btime" */
			token = dyn_str_next_token(dyn_str);
			engine->time_controls[COLOR_BLACK]->time_limit_in_seconds = atoi(token) * 1000;
			break;
		case 0x71c2388517319e0c: /* "winc" */
			token = dyn_str_next_token(dyn_str);
			engine->time_controls[COLOR_WHITE]->increment_in_seconds = atoi(token) * 1000;
			break;
		case 0xad513987341315ae: /* "binc" */
			token = dyn_str_next_token(dyn_str);
			engine->time_controls[COLOR_BLACK]->increment_in_seconds = atoi(token) * 1000;
			break;
		case 0x18ebef875e97de86: /* "infinite" */
			time_control_delete(engine->time_controls[COLOR_WHITE]);
			time_control_delete(engine->time_controls[COLOR_BLACK]);
			break;
	}
	printf("bestmove e2e4 ponder c7c5\n");
}
