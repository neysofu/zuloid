/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_SEARCH_CACHE_H
#define Z64C_SEARCH_CACHE_H

#include "chess/position.h"
#include <stdint.h>
#include <stdlib.h>

/* A fast index implementation with cache capabilities. */
struct Cache;

struct CacheEntry
{
	uint_least16_t data;
};

struct Cache *
cache_new(size_t size_in_bytes);

void
cache_delete(struct Cache *cache);

struct CacheEntry *
cache_get(struct Cache *cache, struct Position *position);

int
cache_insert(struct Cache *cache, struct Position *position, struct CacheEntry *entry);

void
cache_pop(struct Cache *cache);

extern const struct CacheEntry CACHE_ENTRY_EMPTY;

#endif
