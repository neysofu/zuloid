/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "cache/cache.h"
#include "cache/fast_range.h"
#include "globals.h"
#include "switches.h"
#include "utils/utils.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define XXH_INLINE_ALL
#include "xxHash/xxhash.h"

const size_t CACHE_CELL_SIZE = 32;

struct CacheSlot
{
	int32_t signature;
	uint8_t offset;
	uint8_t temperature;
	struct CacheEntry entry;
};

struct Cache
{
	uint8_t temperature_indicator;
	double load_factor;
	size_t size;
	struct CacheSlot *slots;
};

struct Cache *
cache_new(size_t size_in_bytes)
{
	struct Cache *cache = malloc(sizeof(struct Cache));
	if (cache) {
		*cache = (struct Cache){
			.size = size_in_bytes / sizeof(struct CacheSlot),
			.slots = malloc(size_in_bytes + CACHE_CELL_SIZE),
		};
		memset(cache->slots, 0, size_in_bytes + CACHE_CELL_SIZE);
	}
	return cache;
}

void
cache_delete(struct Cache *cache)
{
	if (!cache) {
		return;
	}
	free(cache->slots);
	free(cache);
}

struct CacheEntry *
cache_get(struct Cache *cache, const struct Position *position)
{
	/* 1. Get the correct index based on the position hash.
	 * 2. Iterate in the cell until you find an item that has the same signature and offset.
	 * 3. If found, return it.
	 * 4. Else, the item is not found.
	 * 5. Since it wasn't found, we need to find some space for it.
	 * */
	size_t i;
	switch (ADDRESS_SIZE) {
		case 64:
			i = fast_range_64(XXH64(position, sizeof(struct Position), 0), cache->size);
			break;
		default:
			i = fast_range_32(XXH32(position, sizeof(struct Position), 0), cache->size);
			break;
	}
	uint32_t signature = XXH32(position, sizeof(struct Position), 0);
	struct CacheSlot *slot = &cache->slots[i];
	for (size_t offset = 0; offset < CACHE_CELL_SIZE; offset++, slot++) {
		if (slot->signature == 0) {
			/* Not found. Insert it. */
			slot->signature = signature;
			slot->offset = offset;
			slot->temperature = offset;
			return &slot->entry;
		} else if (slot->signature == signature && slot->offset == offset) {
			/* Found the right entry. */
			return &slot->entry;
		}
	}
	/* The whole cell is filled up. Do some clean up and find it a spot. TODO. */
	return NULL;
}
