/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "cJSON/cJSON.h"
#include "engine.h"
#include "globals.h"
#include "switches.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int
main(void)
{
	setvbuf(stdin, NULL, _IOLBF, 0);
	setvbuf(stdout, NULL, _IOLBF, 0);
	printf("# Z64C/CPU %s (%s)\n", Z64C_VERSION, Z64C_BUILD_DATE);
	printf("# %s\n", Z64C_COPYRIGHT);
	printf("# This is free software; see 'LICENSE.txt' for copying conditions.\n");
	printf("# There is NO warranty of any kind.\n");
	int pid;
	if (get_pid(&pid) == EXIT_SUCCESS) {
		printf("# Process ID: %d\n", pid);
	}
	struct Engine *engine = engine_new();
	struct LineBuffer line_buffer = LINE_BUFFER_EMPTY;
	if (line_buffer_resize(&line_buffer, LINE_BUFFER_DEFAULT_CAPACITY)) {
		return EXIT_FAILURE;
	}
	while (engine->mode != MODE_EXIT) {
		if (read_line(stdin, &line_buffer) == EXIT_SUCCESS) {
			engine_call(engine, line_buffer.string);
		} else {
			engine_delete(engine);
			return EXIT_SUCCESS;
		}
	}
	int exit_status = engine->exit_status;
	engine_delete(engine);
	return exit_status;
}
