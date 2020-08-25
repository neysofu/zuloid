#include "chess/threats.h"
#include "munit/munit.h"
#include <plibsys.h>

void
test_init(void)
{
	PTimeProfiler *timer = p_time_profiler_new();
	init_threats();
	uint64_t elapsed_usecs = p_time_profiler_elapsed_usecs(timer);
	munit_assert_uint(elapsed_usecs, <, 1000ULL * 1000);
}
