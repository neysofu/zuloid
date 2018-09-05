#pragma once

#include <stdint.h>
#include "board.h"

#define TTABLE_BUCKET_SIZE (32768 / sizeof(struct TTableItem))
#define TTABLE_ITEM_INITIAL_NUM_BUCKETS 1

struct TTable;

struct TTable *
ttable_new(void);

void
ttable_drop(struct TTable *ttable);

struct TTableItem *
ttable_ith(struct TTable *ttable, size_t i);

Move
ttable_best_move(struct TTable *ttable);

void
ttable_insert(struct TTable *ttable, struct Board *board);

void
ttable_delete(struct TTable *ttable, uint64_t i);

void
ttable_clear(struct TTable *ttable);

void
ttable_prune_useless_lines(struct TTable *ttable, Move move);
