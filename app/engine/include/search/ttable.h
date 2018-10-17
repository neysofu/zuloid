/**
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
 */
struct TTable;

struct TTable *
ttable_new(void);

void
ttable_free(struct TTable *ttable);

struct TTableNode *
ttable_get(struct TTable *ttable, struct Board *board);

void
ttable_node_backpropagate(struct TTableNode *parent,
                          struct TTableNode *child,
                          bool *lock);

/**
 * @brief The main search procedure.
 * This nonblocking function puts into work every available thread for
 * searching this position until:
 * - told to stop by @p lock;
 * - time is up;
 * - a satisfying move is found.
 */
struct SController *
ttable_search(struct TTable *ttable, struct Board *board);
