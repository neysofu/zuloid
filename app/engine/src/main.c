/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bitboards.h"
#include "engine.h"
#include "trace.h"
#include <stdio.h>
#include <sys/select.h>
#include <sysexits.h>

int
main(void)
{
	setlinebuf(stdin);
	setlinebuf(stdout);
	setlinebuf(stderr);
	TRACE("Line buffering set for all three standard channels.\n");
	bb_init();
	struct Engine *engine = engine_new();
	int8_t exit_status = engine_main(engine);
	engine_free(engine);
	return exit_status;
}
