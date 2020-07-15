#include "cache/cache.h"
#include "munit/munit.h"

void
test_cache_single_key_retrieval(void)
{
	struct Cache *cache = cache_new(1024);
	struct CacheEntry *entry = cache_get(cache, &POSITION_INIT);
	munit_assert_not_null(entry);
}
