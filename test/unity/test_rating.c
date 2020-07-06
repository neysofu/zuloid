#include "Unity/src/unity.h"
#include "rating.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
test_rating(void)
{
	TEST_ASSERT_EQUAL_FLOAT(expected_score(1800, 1800), 0.5);
	TEST_ASSERT_EQUAL_FLOAT(expected_score(2500, 2300), 1.0 - expected_score(2300, 2500));
}
