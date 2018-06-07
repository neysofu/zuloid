#include <stdio.h>
#include <sys/select.h>
#include <sysexits.h>
#include "engine.h"

#define STDIN_BUF_SIZE 256

int
main(void) {
	// Turns on line buffering.
	setvbuf(stdin, NULL, _IOLBF, STDIN_BUF_SIZE);
	struct Engine *engine = engine_new(NULL);
	if (!engine) {
		return EX_OSERR;
	}
	uint8_t exit_code = engine_wait_until_quitting(engine);
	engine_drop(engine);
	return exit_code;
}
