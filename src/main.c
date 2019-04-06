/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "engine.h"
#include "globals.h"
#include "utils/utils.h"
#include "utils/dyn_str.h"
#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
	/* We just want to make sure that line buffering is turned on. */
	setvbuf(stdin, NULL, _IOLBF, 0);
	setvbuf(stdout, NULL, _IOLBF, 0);
	printf("# Z64C/CPU %s (%s)\n", Z64C_VERSION, Z64C_BUILD_DATE);
	printf("# %s\n", Z64C_COPYRIGHT);
	printf("# This is free software; see 'LICENSE.txt' for copying conditions.\n");
	printf("# There is NO warranty of any kind.\n");
	int pid;
	/* There's no point in displaying the PID if it is unavailable. */
	if (get_pid(&pid) == ERR_CODE_NONE) {
		printf("# Process ID: %d\n", pid);
	}
	struct Engine *engine = engine_new();
	if (!engine) {
		return EXIT_FAILURE;
	}
	/* Saves us from countless malloc/free calls. */
	while (engine->mode != MODE_EXIT) {
		struct DynStr dyn_str = DYN_STR_EMPTY;
		if (dyn_str_read_line_from_stream(&dyn_str, stdin)) {
			engine_delete(engine);
			return EXIT_SUCCESS;
		} else {
			engine_call(engine, &dyn_str);
		}
	}
	return engine_delete(engine);
}
