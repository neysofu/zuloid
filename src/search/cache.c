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

struct Cache
{
	size_t size;
	struct CacheEntry entries[];
};

struct Cache *
cache_new(size_t size_in_bytes)
{
	struct Cache *cache = malloc_or_exit(sizeof(struct Cache) + size_in_bytes);
	*cache = (struct Cache){
		.size = size_in_bytes / sizeof(struct CacheEntry),
	};
	memset(cache->entries, 0, size_in_bytes);
	return cache;
}

void
cache_delete(struct Cache *cache)
{
	free(cache);
}

struct CacheEntry *
cache_get(struct Cache *cache, struct Position *position)
{
	assert(cache);
	assert(position);
	size_t i;
	switch (WORD_SIZE) {
		case 64:
			i = fast_range_64(XXH64(position, sizeof(struct Position), 0), cache->size);
			break;
		default:
			i = fast_range_32(XXH32(position, sizeof(struct Position), 0), cache->size);
			break;
	}
	int_fast32_t signature = XXH32(position, sizeof(struct Position), 0);
	struct CacheEntry *entry = &cache->entries[i];
	uint_least8_t min_temperature = UINT_LEAST8_MAX;
	for (size_t offset = 0; offset < 32; offset++, entry++) {
		if (entry->signature == signature && entry->offset == offset) {
			break;
		} else if (entry->temperature < min_temperature) {
			min_temperature = entry->temperature;
		} else if (entry->offset == UINT_LEAST8_MAX) {
			break;
		}
	}
	return entry;
}
