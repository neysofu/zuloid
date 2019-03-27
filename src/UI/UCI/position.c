/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chess/position.h"
#include "UI/cmd.h"
#include "chess/fen.h"
#include "engine.h"
#include <string.h>

void
engine_uci_call_position(struct Engine *engine, struct Cmd *cmd)
{
	if (strcmp(cmd_next(cmd), "startpos") == 0) {
		engine->position = POSITION_DEFAULT;
	} else if (strcmp(cmd_current(cmd), "fen") == 0) {
		position_init_from_fen_as_cmd(&engine->position, cmd);
	}
	// TODO
	// while (cmd_next(&cmd)) {
	//	position_push(string_to_move(cmd_current(&cmd)));
	//}
}
