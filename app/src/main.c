/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "engine.h"

int
main(void)
{
	setvbuf(stdin, NULL, _IOLBF, 0);
	setvbuf(stdout, NULL, _IOLBF, 0);
	struct Engine *engine = engine_new();
	srand(engine->settings.seed);
	int exit_status = engine_main(engine);
	engine_delete(engine);
	return exit_status;
}
