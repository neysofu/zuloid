/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "Z64C.h"
#include "globals.h"
#include "switches.h"
#include "utils.h"
#include <stdio.h>

void
print_welcome_message(void);

int
main(void)
{
	/* Most systems do use line buffering on I/O, but it's not standard. */
	setvbuf(stdin, NULL, _IOLBF, 0);
	setvbuf(stdout, NULL, _IOLBF, 0);
	struct Engine *engine = engine_new();
	print_welcome_message();
	while (!engine_exit_status(engine)) {
		char *line = read_line_from_stream(stdin);
		if (!line) {
			engine_delete(engine);
			return EXIT_SUCCESS;
		}
		char *response = engine_send_request(engine, line);
		if (response) {
			/* A tab character before the response string visually separates requests from
			 * responses in the terminal. */
			printf("\t%s\r\n", response);
			free(response);
		}
	}
	int exit_status = *engine_exit_status(engine);
	engine_delete(engine);
	return exit_status;
}

void
print_welcome_message(void)
{
	printf("# .:.:. Welcome to Z64C .:.:.\r\n");
	printf("# version = %s, nr. bits = %lu", Z64C_VERSION, ARCHITECTURE_BITS);
	struct PID pid = get_pid();
	if (pid.success) {
		printf(", PID = %d", pid.value);
	}
	printf("\r\n");
}
