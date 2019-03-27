/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_CHESS_FEN_H
#define Z64C_CHESS_FEN_H

#include "UI/cmd.h"
#include "chess/position.h"

char *
fen_new_from_position(const struct Position *position);

int
position_init_from_fen_as_cmd(struct Position *position, struct Cmd *cmd);

#endif
