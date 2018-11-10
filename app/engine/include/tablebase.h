/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @file tablebase.h
 * @brief Endgame tablebases probing and initialization.
 */

#pragma once

#include "chess/move.h"
#include <stdlib.h>

struct Tablebase;

int8_t
tablebase_use_gaviota(struct TableBase *tablebase, char *path);

int8_t
tablebase_use_syzygy(struct Tablebase *tablebase, char *path);

Move
tablebase_best_move(struct Tablebase *tablebase, struct Board *board);
