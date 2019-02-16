/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "search/cache.h"
#include "fast_range.h"
#include "globals.h"
#include "switches.h"
#include "utils.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define XXH_INLINE_ALL
#include "xxHash/xxhash.h"

const size_t CACHE_CELL_SIZE = 32;

struct Cache
{
	uint_fast8_t temperature_indicator;
	double load_factor;
	size_t size;
	struct CacheEntry entries[];
};

struct Cache *
cache_new(size_t size_in_bytes)
{
	LOGF("cache_new");
	struct Cache *cache =
	  malloc_or_exit(sizeof(struct Cache) + size_in_bytes + CACHE_CELL_SIZE);
	*cache = (struct Cache){
		.size = size_in_bytes / sizeof(struct CacheEntry),
	};
	memset(cache->entries, 0, size_in_bytes + CACHE_CELL_SIZE);
	return cache;
}

void
cache_delete(struct Cache *cache)
{
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
	LOGF("Entered cache_get");
	size_t i;
	switch (WORD_SIZE) {
		case 64:
			i = fast_range_64(XXH64(position, sizeof(struct Position), 0), cache->size);
			break;
		default:
			i = fast_range_32(XXH32(position, sizeof(struct Position), 0), cache->size);
			break;
	}
	LOGF("i is %zu", i);
	uint_fast32_t signature = XXH32(position, sizeof(struct Position), 0);
	struct CacheEntry *entry = &cache->entries[i];
	for (size_t offset = 0; offset < CACHE_CELL_SIZE; offset++, entry++) {
		if (entry->signature == 0) {
			/* Not found. Insert it. */
			entry->signature = signature;
			entry->offset = offset;
			entry->temperature = offset;
			return entry;
		} else if (entry->signature == signature && entry->offset == offset) {
			/* Found the right entry. */
			return entry;
		}
	}
	/* The whole cell is filled up. Do some clean up and find it a spot. TODO. */
	return NULL;
}
