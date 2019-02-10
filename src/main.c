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

static void
print_welcome_message(void);

int
main(void)
{
	/* Most systems do use line buffering on I/O, but it's not standard. */
	setvbuf(stdin, NULL, _IOLBF, 0);
	setvbuf(stdout, NULL, _IOLBF, 0);
	cJSON_InitHooks(&(struct cJSON_Hooks){ malloc_or_exit, free });
	struct Engine *engine = engine_new();
	print_welcome_message();
	while (engine->mode != MODE_EXIT) {
		char *line = read_line_from_stream(stdin);
		if (!line) {
			engine_delete(engine);
			return EXIT_SUCCESS;
		}
		char *response = engine_call(engine, line);
		if (response) {
			/* A tab character before the response string visually separates requests from
			 * responses in the terminal. */
			printf("\t%s\n", response);
			free(response);
		}
	}
	int exit_status = engine->exit_status;
	engine_delete(engine);
	return exit_status;
}

static void
print_welcome_message(void)
{
	printf("# .:.:. Welcome to Z64C .:.:.\n"
	       "# version = %s, nr. bits = %lu",
	       Z64C_VERSION,
	       ARCHITECTURE_BITS);
	struct PID pid = get_pid();
	if (pid.success) {
		printf(", PID = %d", pid.value);
	}
	putchar('\n');
}
