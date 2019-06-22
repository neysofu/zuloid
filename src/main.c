/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chess/bb.h"
#include "engine.h"
#include "globals.h"
#include "utils.h"
#include <plibsys.h>
#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
	p_libsys_init();
	bb_init();
	/* Let's make sure that line buffering is turned on. */
	setvbuf(stdin, NULL, _IOLBF, 0);
	setvbuf(stdout, NULL, _IOLBF, 0);
	/* Now printing a welcome message for CLI users...
	 * The number sign ensures minimal possibility of accidental evaluation and it just
	 * happens to use the same syntax as CECP's debug messages. */
	printf("# Z64C/%s %s (%s)\n", Z64C_BACKEND_NAME, Z64C_VERSION, Z64C_BUILD_DATE);
	printf("# %s\n", Z64C_COPYRIGHT);
	printf("# This is free software; see 'LICENSE.txt' for copying conditions.\n");
	printf("# There is NO warranty of any kind.\n");
	printf("# Process ID: %d\n", p_process_get_current_pid());
	struct Engine *engine = engine_new();
	if (!engine) {
		return EXIT_FAILURE;
	}
	do {
		char *line = NULL;
		size_t foo = 0;
		if (read_line(&line, &foo, stdin) == -1) {
			free(line);
			engine_delete(engine);
			return EXIT_FAILURE;
		} else {
			engine_call(engine, line);
		}
	} while (engine->mode != MODE_EXIT);
	p_libsys_shutdown();
	return engine_delete(engine);
}
