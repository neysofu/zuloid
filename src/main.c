#include "chess/bb.h"
#include "engine.h"
#include "globals.h"
#include "mt-64/mt-64.h"
#include "utils.h"
#include <plibsys.h>
#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
	init_subsystems();
	// The number sign ensures minimal possibility of accidental evaluation by
	// the client.
	printf("# Zorro %s (%s)\n", ZORRO_VERSION_VERBOSE, ZORRO_BUILD_DATE);
	printf("# Copyright (c) 2018-2020 Filippo Costa\n");
	printf("# Process ID: %d\n", p_process_get_current_pid());
	struct Engine *engine = engine_new();
	while (engine->status != STATUS_EXIT) {
		char *line = read_line(stdin);
		engine->protocol(engine, line);
		free(line);
	}
	engine_delete(engine);
	p_libsys_shutdown();
	return EXIT_SUCCESS;
}
