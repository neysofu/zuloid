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

void
print_welcome_message(void);

int
main(void)
{
	setvbuf(stdin, NULL, _IOLBF, 0);
	setvbuf(stdout, NULL, _IOLBF, 0);
	struct Engine *engine = engine_new();
	print_welcome_message();
	struct LineBuffer *line_buffer = NULL;
	line_buffer_resize(line_buffer, LINE_BUFFER_DEFAULT_CAPACITY);
	while (engine->mode != MODE_EXIT) {
		if (read_line_from_stream(stdin, line_buffer)) {
			engine_call(engine, line_buffer->string);
		} else {
			engine_delete(engine);
			return EXIT_SUCCESS;
		}
	}
	int exit_status = engine->exit_status;
	engine_delete(engine);
	return exit_status;
}

void
print_welcome_message(void)
{
	printf("# Z64C/CPU %s (%s)\n", Z64C_VERSION, Z64C_BUILD_DATE);
	printf("# %s\n", Z64C_COPYRIGHT);
	printf("# This is free software; see 'LICENSE.txt' for copying conditions.\n");
	printf("# There is NO warranty of any kind.\n");
	struct PID pid = get_pid();
	if (pid.success) {
		printf("# Process ID: %d\n", pid.value);
	}
}
