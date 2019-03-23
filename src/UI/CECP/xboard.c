/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "UI/cmd.h"
#include "engine.h"
#include "utils.h"

void
engine_cecp_call_xboard(struct Engine *engine, struct Cmd *cmd)
{
	UNUSED(engine);
	UNUSED(cmd);
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
