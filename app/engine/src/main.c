#include <stdio.h>
#include <sys/select.h>
#include <sysexits.h>
#include "engine.h"
#include "globals.h"

int
main(void) {
	setlinebuf(stdin);
	setlinebuf(stdout);
	setlinebuf(stderr);
	globals_init();
	struct Engine *engine = engine_new();
	if (!engine) {
		return EX_OSERR;
	}
	int exit_code = engine_main(engine);
	engine_drop(engine);
	return exit_code;
}
