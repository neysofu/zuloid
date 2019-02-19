/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "search/cache.h"
#include "Unity/src/unity.h"

void
test_cache_single_key_retrieval(void)
{
	struct Cache *cache = cache_new(1024);
	struct CacheEntry *entry = cache_get(cache, &POSITION_DEFAULT);
	TEST_ASSERT_NOT_NULL(entry);
}
