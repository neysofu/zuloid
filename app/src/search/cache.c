/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "search/cache.h"
#include "globals.h"
#include "switches.h"
#include "utils.h"
#include "xxHash/xxhash.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#if defined(SWITCH_OPENMP)
#include <omp.h>
#endif

/* You may change this to have a cache that uses smaller blocks. Something
 * between 0 and 10 is ideal. */
const size_t CACHE_FRAGMENTATION_FACTOR = 6;
const size_t CACHE_BLOCKS_COUNT = 1U << CACHE_FRAGMENTATION_FACTOR;

struct Cache
{
	size_t entries_count_per_block;
	struct CacheEntry *blocks[CACHE_BLOCKS_COUNT];
};

struct Cache *
cache_new(size_t size)
{
	struct Cache *cache = handle_oom(malloc(sizeof(struct Cache)));
	cache->entries_count_per_block = size >> CACHE_FRAGMENTATION_FACTOR;
	memset(cache->blocks, 0, sizeof(cache->blocks));
	for (size_t i = 0; i < CACHE_BLOCKS_COUNT; i++) {
		cache->blocks[i] =
		  malloc(sizeof(struct CacheEntry) * cache->entries_count_per_block);
		if (!cache->blocks[i]) {
			cache_delete(cache);
			return NULL;
		}
	}
	return cache;
}

void
cache_delete(struct Cache *cache)
{
	if (!cache) {
		return;
	}
	for (size_t i = 0; i < CACHE_BLOCKS_COUNT; i++) {
		free(cache->blocks[i]);
	}
	free(cache);
}

struct CacheEntry *
cache_ith_entry(const struct Cache *cache, size_t i)
{
	assert(cache);
	return cache->blocks[i & (CACHE_BLOCKS_COUNT - 1)] + (i >> CACHE_FRAGMENTATION_FACTOR);
}

struct CacheEntry *
cache_probe(const struct Cache *cache, uint_fast64_t key)
{
	assert(cache);
	return NULL;
}
