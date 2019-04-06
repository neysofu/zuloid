/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "engine.h"
#include "utils/dyn_str.h"
#include "xxHash/xxhash.h"
#include <assert.h>
#include <ctype.h>
#include <string.h>

void
engine_uci_call_setoption(struct Engine *engine, struct DynStr *dyn_str)
{
	assert(engine);
	assert(dyn_str);
	dyn_str_next_token(dyn_str);
	uint64_t hash = 0;
	char *token = dyn_str_next_token(dyn_str);
	while (strcmp(token, "value") != 0) {
		for (size_t i = 0; token[i]; i++) {
			token[i] = tolower(token[i]);
		}
		hash ^= XXH64(token, strlen(token), 0);
	}
	dyn_str_next_token(dyn_str);
	switch (hash) {
		case 0xd8cdd8e8314c4147: /* "hash" */
			break;
		case 0xc057a61296095806: /* "nalimovpath" */
			break;
		case 0xcfaf77aca1f8feaa: /* "nalimovcache" */
			break;
		case 0x0a6f394a3987568a: /* "ponder" */
			break;
		case 0x487e0e93e2c2bb18: /* "ownbook" */
			break;
		case 0xc8922f8e470ffaee: /* "uci_showcurrline" */
			break;
		case 0xdf718624bc46e2ce: /* "uci_showrefutations" */
			break;
		case 0xf6253b92eb36d560: /* "uci_limitstrength" */
			break;
	}
}
