/// @file ttable.h
/// @brief In-memory transpositions table.

#pragma once

#include <stdint.h>
#include "chess/board.h"
#include "chess/move.h"

// A transposition table can store a huge number of chess positions with
// candidate moves and metadata. The internal memory layout is a hashed array
// tree with Robin Hood open addressing. Memory fragments' size can be chosen to
// fit into a single huge page to reduce TLB misses.
// Some use cases require specific data to be stored along chess positions, so
// transposition tables support cells of any size.
struct TTable;

struct TTable *
ttable_new(void);

void
ttable_drop(struct TTable *ttable);

void
ttable_clear_fragments(struct TTable *ttable);

void *
ttable_ith_fragment(struct TTable *ttable, size_t i);

void *
ttable_element(struct TTable *ttable, struct Board *board);
