#include "chess/bb.h"
#include "engine.h"
#include "globals.h"
#include "protocols/uci.h"
#include "utils.h"
#include <plibsys.h>
#include "mt-64/mt-64.h"
#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
	init_genrand64(0xd895643full);
	p_libsys_init();
	// UCI (and CECP as well, for that matter) is a line-oriented protocol; so
	// we want to turn line buffering on.
	setvbuf(stdin, NULL, _IOLBF, 0);
	setvbuf(stdout, NULL, _IOLBF, 0);
	// The number sign ensures minimal possibility of accidental evaluation.
	printf("# Zorro %s (%s)\n", ZORRO_VERSION_VERBOSE, ZORRO_BUILD_DATE);
	printf("# Copyright (c) 2018-2020 Filippo Costa\n");
	printf("# Process ID: %d\n", p_process_get_current_pid());
	struct Engine *engine = engine_new();
	while (engine->status != STATUS_EXIT) {
		char *line = read_line();
		protocol_uci_handle(engine, line);
		free(line);
	}
	engine_delete(engine);
	p_libsys_shutdown();
	return EXIT_SUCCESS;
}
