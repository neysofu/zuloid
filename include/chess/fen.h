/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_CHESS_FEN_H
#define Z64C_CHESS_FEN_H

#include "chess/position.h"

char *
fen_new_from_position(const struct Position *position);

int
position_set_from_fen(struct Position *position, const char *fen);

#endif