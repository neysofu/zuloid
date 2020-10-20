/* SPDX-License-Identifier: GPL-3.0-only */

#include "cache/cache.h"
#include "cache/fast_range.h"
#include "feature_flags.h"
#include "utils.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define XXH_INLINE_ALL
#include "xxHash/xxhash.h"

const size_t CACHE_BUCKET_SIZE = 32;

const int ADDRESS_SIZE = sizeof(void *) * CHAR_BIT;

/* We strive for 8 bytes for each position. */
struct CacheSlot
{
	/* 32 bits ought to be more than enough to solve collisions. */
	int32_t signature;
	/* Displacement count from the ideal cache slot. */
	uint8_t probe_count;
	/* The temperature parameter controls the cache eviction policy. */
	uint8_t temperature;
	struct CacheEntry entry;
};

struct Cache
{
	uint8_t temperature_indicator;
	double load_factor;
	size_t capacity;
	struct CacheSlot *slots;
};

void
cache_init_slots(struct Cache *cache)
{
	memset(cache->slots, 0, cache->capacity / sizeof(struct CacheSlot) + CACHE_BUCKET_SIZE);
}

struct Cache *
cache_new(size_t size_in_bytes)
{
	struct Cache *cache = malloc(sizeof(struct Cache));
	exit_if_null(cache);
	*cache = (struct Cache){
		.capacity = size_in_bytes / sizeof(struct CacheSlot),
		.slots = malloc(size_in_bytes + CACHE_BUCKET_SIZE),
	};
	cache_init_slots(cache);
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

size_t
cache_clear(struct Cache *cache)
{
	cache_init_slots(cache);
	return 0;
}

struct CacheEntry *
cache_get(struct Cache *cache, const struct Board *position)
{
	/* 1. Get the correct index based on the position hash.
	 * 2. Iterate in the cell until you find an item that has the same signature
	 * and probe_count.
	 * 3. If found, return it.
	 * 4. Else, the item is not found.
	 * 5. Since it wasn't found, we need to find some space for it.
	 * */
	size_t i;
	switch (ADDRESS_SIZE) {
		case 64:
			i = fast_range_64(XXH64(position, sizeof(struct Board), 0), cache->capacity);
			break;
		default:
			/* Fall back to 32 bits for everything else. */
			i = fast_range_32(XXH32(position, sizeof(struct Board), 0), cache->capacity);
			break;
	}
	/* TODO: reuse the previous hash operation. */
	int32_t signature = XXH32(position, sizeof(struct Board), 0);
	struct CacheSlot *slot = cache->slots + i;
	for (size_t probe_count = 0; probe_count < CACHE_BUCKET_SIZE; probe_count++, slot++) {
		if (slot->signature == 0) {
			/* The item was not found. Insert it and return. */
			slot->signature = signature;
			slot->probe_count = probe_count;
			slot->temperature = probe_count;
			return &slot->entry;
		} else if (slot->signature == signature && slot->probe_count == probe_count) {
			/* Found the right entry. */
			return &slot->entry;
		} else if (slot->signature) {
		}
	}
	/* The whole bucket is filled up. Do some clean up and find it a spot. TODO.
	 */
	return NULL;
}
