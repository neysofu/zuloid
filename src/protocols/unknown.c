/* Copyright (c) Filippo Costa - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * PROPRIETARY AND CONFIDENTIAL */

#include "engine.h"
#include "protocols.h"
#include <assert.h>
#include <string.h>

void
engine_unknown_protocol(struct Engine *engine, char *cmd)
{
	if (strstr(cmd, "uci")) {
		engine->protocol = PROTOCOL_UCI;
	} else if (strstr(cmd, "xboard")) {
		engine->protocol = PROTOCOL_CECP;
	} else {
		printf("# Unsupported engine protocol. Abort.\n");
		engine->mode = MODE_EXIT;
		engine->exit_status = EXIT_FAILURE;
		return;
	}
	engine_call(engine, cmd);
}
