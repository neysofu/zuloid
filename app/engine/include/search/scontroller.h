/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @file scontroller.h
 * @brief Search controllers for high-level search operations.
 */

#pragma once

#include "search/ttable.h"
#include <stdint.h>

struct SController;

/**
 * @brief Create a new search controller based on the transpositions from @p
 * ttable.
 */
struct SController *
scontroller_new(struct TTable *ttable);

/**
 * @brief Free @p scontroller and run destructors.
 */
void
scontroller_free(struct SController *scontroller);

/**
 * @brief Start searching with all the workers available.
 */
void
scontroller_start(struct SController *scontroller,
                  struct Board *board);

/**
 * @brief Stop searching.
 * @return The best move found.
 */
Move
scontroller_stop(struct SController *scontroller);
