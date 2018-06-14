#include <stdio.h>
#include <sys/select.h>
#include <sysexits.h>
#include "engine.h"

int
main(void) {
	setvbuf(stdin, NULL, _IOLBF, 0);
	setvbuf(stdout, NULL, _IOLBF, 0);
	struct Engine *engine = engine_new(NULL);
	if (!engine) {
		return EX_OSERR;
	}
	uint8_t exit_code = engine_main(engine);
	engine_drop(engine);
	return exit_code;
}
