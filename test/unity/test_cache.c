/* Copyright (c) Filippo Costa - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * PROPRIETARY AND CONFIDENTIAL */

#include "cache/cache.h"
#include "Unity/src/unity.h"

void
test_cache_single_key_retrieval(void)
{
	struct Cache *cache = cache_new(1024);
	struct CacheEntry *entry = cache_get(cache, &POSITION_INIT);
	TEST_ASSERT_NOT_NULL(entry);
}
