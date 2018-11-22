/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bitboards.h"
#include "cJSON/cJSON.h"
#include "engine.h"
#include "trace.h"
#include "utils.h"
#include <stdio.h>
#include <sys/select.h>

int
main(void)
{
	/* The communication protocol over stdin/stdout is line-oriented, so we
	 * make sure that line buffering is on. */
	setlinebuf(stdin);
	setlinebuf(stdout);
	TRACE("Line buffering set for all three standard channels.\n");
	/* Initialize bitboard lookup tables. */
	bb_init();
	/* Let's make cJSON use xmalloc too for better consistency in case of
	 * out-of-memory errors. */
	struct cJSON_Hooks cjson_hooks = { xmalloc, free };
	cJSON_InitHooks(&cjson_hooks);
	struct Engine *engine = engine_new();
	int8_t exit_status = engine_main(engine);
	engine_free(engine);
	return exit_status;
}
