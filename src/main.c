/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

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
	do {
		char *line = NULL;
		size_t foo = 0;
		if (getline(&line, &foo, stdin) == -1) {
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
