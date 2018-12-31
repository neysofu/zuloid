/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_SEARCH_CACHE_H
#define Z64C_SEARCH_CACHE_H

#include <stdint.h>
#include <stdlib.h>

/* The cache stores search results
 * 
 * */
struct Cache;

struct CacheEntry
{
	uint_least16_t score;
	uint_least8_t replacement_factor;
};

struct Cache *
cache_new(size_t size);

void
cache_delete(struct Cache *cache);

struct CacheEntry *
cache_ith_entry(const struct Cache *cache, size_t i);

struct CacheEntry *
cache_probe(const struct Cache *cache, uint_fast64_t key);

struct CacheEntry *
cache_probe_nonexistant(struct Cache *cache, uint_fast64_t key);

void
cache_pop(struct Cache *cache, uint_fast64_t key);

#endif
