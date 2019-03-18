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
	cJSON_InitHooks(&(cJSON_Hooks){ malloc_or_exit, free });
	struct Engine *engine = engine_new();
	print_welcome_message();
	printf("# Copy and paste to init: {\"method\":\"init\",\"id\":0}\n");
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

void
print_jsonrpc_response(const char *response)
{
	printf("\t%s\n", response);
}

static void
print_welcome_message(void)
{
	printf("# Welcome to Z64C %s (%s)\n", Z64C_VERSION, Z64C_RELEASE_DATE_ISO_8601);
	printf("# word size = %d\n", WORD_SIZE);
	struct PID pid = get_pid();
	if (pid.success) {
		printf("# PID = %d\n", pid.value);
	}
}
