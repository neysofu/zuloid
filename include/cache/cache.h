#ifndef ZORRO_CACHE_CACHE_H
#define ZORRO_CACHE_CACHE_H

#include "chess/position.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

struct Cache;

struct CacheEntry
{
	uint8_t evaluation;
	uint8_t dispersion;
};

struct Cache *
cache_new(size_t size_in_bytes);

struct CacheEntry *
cache_get(struct Cache *cache, const struct Board *pos);

bool
cache_entry_is_empty(struct CacheEntry *entry);

size_t
cache_clear(struct Cache *cache);

void
cache_delete(struct Cache *cache);

#endif
