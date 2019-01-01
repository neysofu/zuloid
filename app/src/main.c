/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "Z64C.h"
#include "globals.h"
#include "utils.h"
#include <stdio.h>

void
print_welcome_message(void);

int
main(void)
{
	setvbuf(stdin, NULL, _IOLBF, 0);
	setvbuf(stdout, NULL, _IOLBF, 0);
	struct Engine *engine = engine_new();
	print_welcome_message();
	while (!engine_exit_status(engine)) {
		char *line = read_line_from_stream(stdin);
		if (!line) {
			printf("{\"jsonrpc\":\"2.0\",\"id\":null,\"error\":{\"code\":0,\"message\":"
			       "\"Input error\"}}\r\n");
			engine_delete(engine);
			return EXIT_FAILURE;
		}
		char *response = engine_send_request(engine, line);
		if (response) {
			/* A tab character before the response string visually separates requests from
			 * responses in the terminal. */
			printf("\t%s\r\n", response);
			free(response);
		}
	}
	const int exit_status = *engine_exit_status(engine);
	engine_delete(engine);
	return exit_status;
}

void
print_welcome_message(void)
{
	printf("# .:.:. Welcome to Z64C .:.:.\r\n");
	printf("# version = %s, nr. bits = %lu, release date = %s",
	       Z64C_VERSION,
	       ARCHITECTURE_BITS,
	       Z64C_RELEASE_DATE_ISO_8601);
	struct PID pid = get_pid();
	if (pid.success) {
		printf(", PID = %d", pid.value);
	}
	printf("\r\n");
}
