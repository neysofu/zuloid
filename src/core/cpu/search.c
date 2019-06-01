/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "engine.h"
#include <plibsys.h>
#include <stdio.h>

void
engine_start_search(struct Engine *engine)
{
	ENGINE_LOGF(engine, "Started searching on a new thread.\n");
}

void
engine_stop_search(struct Engine *engine)
{
	ENGINE_LOGF(engine, "Exiting search thread...\n");
	p_uthread_exit(EXIT_SUCCESS);
}
