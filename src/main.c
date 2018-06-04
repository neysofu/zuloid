#include <stdio.h>
#include <sys/select.h>
#include "engine.h"

int
main(void) {
	// The UCI protocol is line-oriented and forbids any command that doesn's
	// end with a line break, so we make sure that line buffering is turned on
	// on STDIN.
	setvbuf(stdin, NULL, _IOLBF, STDIN_BUF_SIZE);
	struct Engine *engine = engine_new();
	if (!engine) {
		return EX_OSERR;
	} else if (engine->err) {
		return engine->exit_code;
	}
	engine_wait(engine);
	exit_code = engine_wait_until_quitting(engine);
	engine_drop(engine);
	return exit_code;
}
