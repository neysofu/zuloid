#include "Unity/src/unity.h"
#include "cache/cache.h"

void
test_cache_single_key_retrieval(void)
{
	struct Cache *cache = cache_new(1024);
	struct CacheEntry *entry = cache_get(cache, &POSITION_INIT);
	TEST_ASSERT_NOT_NULL(entry);
}
