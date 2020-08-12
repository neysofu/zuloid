#include "engine.h"
#include "globals.h"
#include "utils.h"
#include <plibsys.h>
#include <stdio.h>
#include <stdlib.h>

struct Engine engine;

int
main(int argc, char **argv)
{
	init_subsystems();
	engine_init(&engine);
	// The number sign ensures minimal possibility of accidental evaluation by
	// the client.
	printf("# Zuloid %s (%s)\n", ZULOID_VERSION_VERBOSE, ZULOID_BUILD_DATE);
	printf("# Copyright (c) 2018-2020 Filippo Costa\n");
	printf("# Process ID: %d\n", p_process_get_current_pid());
	for (int i = 1; i < argc && engine.status != STATUS_EXIT; i++) {
		engine.protocol(&engine, argv[i]);
	}
	while (engine.status != STATUS_EXIT) {
		char *line = read_line(stdin);
		engine.protocol(&engine, line);
		free(line);
	}
	p_libsys_shutdown();
	return EXIT_SUCCESS;
}
