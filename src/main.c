#include "chess/bb.h"
#include "engine.h"
#include "globals.h"
#include "protocols/uci.h"
#include "utils.h"
#include <plibsys.h>
#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
	p_libsys_init();
	bb_init();
	setvbuf(stdin, NULL, _IOLBF, 0);
	setvbuf(stdout, NULL, _IOLBF, 0);
	// Now printing a welcome message for terminal users...
	// The number sign ensures minimal possibility of accidental evaluation and it just
	// happens to use the same syntax as CECP's debug messages.
	printf("# Zorro/%s %s (%s)\n", ZORRO_BACKEND_NAME, ZORRO_VERSION, ZORRO_BUILD_DATE);
	printf("# Copyright (c) 2018-2020 Filippo Costa\n");
	printf("# Process ID: %d\n", p_process_get_current_pid());
	struct Engine *engine = engine_new();
	while (engine->mode != MODE_EXIT) {
		char *line = read_line();
		protocol_uci_handle(engine, line);
		free(line);
	}
	p_libsys_shutdown();
	engine_delete(engine);
	return EXIT_SUCCESS;
}
