#include <stdio.h>
#include <sys/select.h>
#include <sysexits.h>
#include "engine.h"
#include "settings.h"

int
main(void) {
	setvbuf(stdin, NULL, _IOLBF, 0);
	setvbuf(stdout, NULL, _IOLBF, 0);
	settings_init();
	struct Engine *engine = engine_new(NULL);
	if (!engine) {
		return EX_OSERR;
	}
	int exit_code = engine_main(engine);
	engine_drop(engine);
	return exit_code;
}
