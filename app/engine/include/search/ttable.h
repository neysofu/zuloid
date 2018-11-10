/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @file ttable.h
 * @brief In-memory transpositions table.
 */

#pragma once

#include "chess/board.h"
#include "chess/move.h"
#include "search/ttable_node.h"
#include "switches.h"
#include <stdint.h>

/**
 * It can store a huge number of chess positions and keep them
 * sorted for quick access.
 *
 * Elements of the transpositions table are:
 * - Play hierarchy. Basically contains nodes with probabilities (approximately
 *   ordered by strength). NB the behaviour of nodes once searched is very
 *   clear.
 *   What I want to do is improve data locality... how the hell can I do that?
 *   Maybe I already got it as much as possible though... Temporal locality is
 *   there by design. Now I need an algorithm that is cache-aware and loads only
 *   most important cells, each that fits into a single cache.
 *   with a bit that are terminal
 * - Position database. This tells me where to find a certain position.
 *   Implemented by binary tree search and possibly accelerated with Cuckoo
 *   filters.
 * - TensorIn. Not all positions have tensor_in, because they are only needed in
 *   terminal nodes. TensorIn database are just pointers from a stack-like
 *   memory pool.
 */
struct TTable;

/**
 * @brief Create an empty transpositions table.
 * @return A pointer to the new data structure.
 */
struct TTable *
ttable_new(void);

void
ttable_free(struct TTable *ttable);

/**
 * @brief Remove any stored transpositions from @ttable and resize it to the
 * default size.
 */
void
ttable_clear(struct TTable *ttable);

struct TTableNode *
ttable_probe(struct TTable *ttable, uint64_t key);
