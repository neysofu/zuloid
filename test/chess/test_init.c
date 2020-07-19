#include "chess/bb.h"
#include "munit/munit.h"
#include <plibsys.h>

void
test_init(void)
{
	PTimeProfiler *timer = p_time_profiler_new();
	bb_init();
	uint64_t elapsed_usecs = p_time_profiler_elapsed_usecs(timer);
	munit_assert_uint(elapsed_usecs, <, 1000000ULL);
}
