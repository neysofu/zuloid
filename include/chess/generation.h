/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_CHESS_GENERATION_H
#define Z64C_CHESS_GENERATION_H

#include "chess/position.h"
#include "chess/move.h"

size_t
position_gen_pseudolegal_moves(const struct Position *position, struct Move buffer[]);

size_t
position_gen_legal_moves(const struct Position *position, struct Move buffer[]);

#endif
