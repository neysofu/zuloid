#include "munit/munit.h"
#include "rating.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
test_rating(void)
{
	munit_assert_double_equal(expected_score(1800, 1800), 0.5, 4);
	munit_assert_double_equal(expected_score(0, 0), 0.5, 4);
	munit_assert_double_equal(expected_score(-500, -500), 0.5, 4);
	munit_assert_double_equal(
	  expected_score(2500, 2300), 1.0 - expected_score(2300, 2500), 4);
}
