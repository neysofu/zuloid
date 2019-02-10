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
#include <stdlib.h>
#include <string.h>

#define XXH_INLINE_ALL
#include "xxHash/xxhash.h"

struct Cache
{
	size_t size;
	struct CacheEntry *entries;
};

struct Cache *
cache_new(size_t size_in_bytes)
{
	struct Cache *cache = malloc_or_exit(sizeof(struct Cache));
	*cache = (struct Cache){
		.size = size_in_bytes / sizeof(struct CacheEntry), .entries = malloc(size_in_bytes),
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
	uint_fast64_t hash = XXH64(position, sizeof(struct Position), 0);
	int_fast32_t signature = XXH32(position, sizeof(struct Position), 0);
	size_t i = fast_range_64(hash, cache->size);
	struct CacheEntry *entry = cache->entries + i;
	while (entry->signature == signature || entry->signature == 0) {
		entry++;
	}
	return entry;
}
