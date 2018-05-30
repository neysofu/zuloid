#include <stdio.h>
#include "xxhash.h"
#include "uci.h"

int
main(int argc, char *argv[]) {
	struct Engine *engine = engine_init();
	engine_establish_connection(engine);
	return engine_wait(engine);
}
