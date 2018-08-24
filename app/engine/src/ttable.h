#pragma once

#include <stdint.h>
#include "board.h"

#define TTABLE_ITEM_BUCKET_SIZE (32768 / sizeof(struct TTableItem))
#define TTABLE_ITEM_NUM_BUCKETS 1

struct TTable;

struct TTableItem;

struct TTable *
ttable_new(void);

void
ttable_drop(struct TTable *ttable);

void
ttable_insert(struct TTable *ttable, struct Board *board);

void
ttable_delete(struct TTable *ttable, uint64_t i);
