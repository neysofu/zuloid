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
#include "eval/eval_state.h"
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
 * A transposition. Should fit into as little cache lines as possible.
 *
 * Note: transposition tables don't need to store the chess position at all!
 * This is because the search tree is completely blind, and only supports
 * indirect querying and hash collisions are dealts with children nodes.
 *
 * Nodes should be searcheable for when a new node comes up with the same
 * position and, before inserting the new node, should be able to find out if it
 * exists just yet. What to do at that point?
 * - Check a hash table? It's great, no cache misses, but doesn't scale.
 *
 * There are two different kind of nodes:
 * - Inner nodes, which must provide the board tensors.
 * - Outer nodes.
 *
 * The only search happens at outer nodes, so they're the ones that need board
 * tensors. It's gonna be very resource intensive if every single move is given
 * a board tensor... but let's not forget that I do ~220 moves in one. So, every
 * node exploration triggers a
 */
struct TTableNode
{
	struct EvalState eval_state;
};

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

void
ttable_remove(struct TTable *ttable, uint64_t key);

struct TTableNode *
ttable_insertion(struct TTable *ttable, uint64_t key);
