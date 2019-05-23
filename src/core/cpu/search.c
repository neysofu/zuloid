/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "engine.h"
#include <plibsys.h>
#include <stdio.h>

int
engine_search(struct Engine *engine)
{
	ENGINE_LOGF(engine, "Started searching on a new thread.");
	p_uthread_exit(EXIT_SUCCESS);
	return 0;
}
