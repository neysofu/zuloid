/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "Z64C.h"
#include "engine.h"
#include "utils.h"

int
main(void)
{
	setvbuf(stdin, NULL, _IOLBF, 0);
	setvbuf(stdout, NULL, _IOLBF, 0);
	struct Engine *engine = engine_new();
	print_welcome_message();
	while (engine->mode != MODE_EXIT) {
		char *str = read_line_from_stream(stdin);
		if (!str) {
			printf("{\"jsonrpc\":\"2.0\",\"id\":null,\"error\":{\"code\":0,\"message\":"
			       "\"Input error\"}}\r\n");
			engine_delete(engine);
			return EXIT_FAILURE;
		}
		char *response = engine_call(engine, str);
		if (response) {
			/* A tab character before the response string visually separates requests from
			 * responses in the terminal. */
			printf("\t%s\r\n", response);
			free(response);
		}
	}
	engine_delete(engine);
	return EXIT_SUCCESS;
}
