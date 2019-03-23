/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "UI/cmd.h"
#include "engine.h"
#include "utils.h"

void
engine_uci_call_uci(struct Engine *engine, struct Cmd *cmd)
{
	UNUSED(engine);
	UNUSED(cmd);
	printf("id name Z64C\n");
	printf("id author Filippo Costa\n");
	printf("uciok\n");
}
