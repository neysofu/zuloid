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

const size_t CACHE_COLD_ENTRIES_COUNT = 64;
const size_t CACHE_DELTA_INDICATORS_COUNT = 4;

/* The cache is a locality-sensitive hash table. The basic idea is that we use
 * fancy tricks to bring everything closer until it's no longer necessary and
 * it would just destroy performance. At that point we've got a CELL, and
 * that's where you can just use hashes. We you a special algorithm to decide
 * which cell a certain element goes to. This algorithm is very fast and
 * touches little memory points. The whole point of having a good cell
 * hierarchy is to spread data without filling too much in a certain region.
 *
 * Basically I want to know where the hell to put elements. I could use a
 * probing method: I calculate the locality sensitive hash and I make it more
 * and more "random" until there's enough space. */
struct Cache
{
	size_t size;
	struct CacheEntry root;
	struct CacheEntry *entries;
};

struct Cache *
cache_new(size_t size_in_bytes)
{
	struct Cache *cache = malloc_or_exit(sizeof(struct Cache));
	*cache = (struct Cache){
		.size = size_in_bytes / sizeof(struct CacheEntry),
		.root = CACHE_ENTRY_EMPTY,
		.entries = malloc_or_exit(size_in_bytes),
	};
	memset(cache->entries, 0, size_in_bytes);
	return cache;
}

void
cache_delete(struct Cache *cache)
{
	if (!cache) {
		return;
	}
	free(cache->entries);
	free(cache);
}

struct CacheEntry *
cache_ith_entry(struct Cache *cache, size_t i)
{
	assert(cache);
	assert(i < cache->size);
	return cache->entries + i;
}

struct CacheEntry *
cache_get(struct Cache *cache, struct Position *position)
{
	assert(cache);
	uint_fast64_t hash = XXH64(position, sizeof(struct Position), 0);
	struct CacheEntry *entry = cache->entries + (hash % cache->size);
	for (int i = 0; i != entry->data; i++);
	return entry;
}

struct CacheEntry *
cache_get_(struct Cache *cache, struct Position *position)
{
	assert(cache);
	uint_fast64_t hash = XXH64(position, sizeof(struct Position), 0);
	struct CacheEntry *entry = cache->entries + (hash % cache->size);
	for (int i = 0; i != entry->data; i++);
	return entry;
}

const struct CacheEntry CACHE_ENTRY_EMPTY = { 0 };
