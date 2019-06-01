/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_SEARCH_CACHE_H
#define Z64C_SEARCH_CACHE_H

#include "chess/position.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* A fast associative array with caching behavior for chess positions.
 * It is optimized for costly cache misses and low memory overhead. */
struct Cache;

struct CacheEntry
{
	uint8_t evaluation;
	uint8_t dispersion;
};

struct Cache *
cache_new(size_t size_in_bytes);

struct CacheEntry *
cache_get(struct Cache *cache, const struct Position *position);

bool
cache_entry_is_empty(struct CacheEntry *entry);

/* Removes all stored positions from the cache and returns the number of deleted entries. */
size_t
cache_clear(struct Cache *cache);

void
cache_delete(struct Cache *cache);

#endif
