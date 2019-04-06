/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "engine.h"
#include "utils/dyn_str.h"
#include <assert.h>

void
engine_cecp_call_xboard(struct Engine *engine, struct DynStr *dyn_str)
{
	assert(engine);
	assert(dyn_str);
	printf("feature myname=\"Z64C\"\n"
	       "feature ping=1\n"
	       "feature playother=1\n"
	       "feature memory=1\n"
	       "feature usermove=0\n"
	       "feature setboard=1\n"
	       "feature draw=1\n"
	       "feature nps=1\n"
	       "feature done=1\n");
}
