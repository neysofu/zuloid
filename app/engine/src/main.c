#include <stdio.h>
#include <sys/select.h>
#include <sysexits.h>
#include "bitboards.h"
#include "engine.h"
#include "globals.h"
#include "settings.h"

uint_fast8_t
main(void) {
	setvbuf(stdin, NULL, _IOLBF, 0);
	setvbuf(stdout, NULL, _IOLBF, 0);
	globals_init();
	bb_init();
	struct Engine *engine = engine_new();
	if (!engine) {
		return EX_OSERR;
	}
	// Here is when Z64C actually gets responsive to UCI commands.
	uint_fast8_t exit_code = engine_main(engine);
	engine_drop(engine);
	return exit_code;
}
