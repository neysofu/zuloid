/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "utils.h"
#include "Unity/src/unity.h"
#include <time.h>
#include <unistd.h>

void
test_utils(void)
{
	TEST_ASSERT_EQUAL_INT(1337, MAX(42, 1337));
	TEST_ASSERT_EQUAL_INT(1337, MAX(1337, 42));
	TEST_ASSERT_EQUAL_INT(42, MAX(-1337, 42));
	TEST_ASSERT_EQUAL_INT(-2, MAX(-2, -2));
	TEST_ASSERT_EQUAL_INT(42, MIN(42, 1337));
	TEST_ASSERT_EQUAL_INT(42, MIN(1337, 42));
	TEST_ASSERT_EQUAL_INT(-1, MIN(-1, 0));
	TEST_ASSERT_EQUAL_INT(3, MIN(3, 3));
	TEST_ASSERT_INT_WITHIN(1, time(NULL), util_timestamp_msec() / 1000);
	TEST_ASSERT(util_str_is_whitespace(" \t\r\n\v"));
	TEST_ASSERT_FALSE(util_str_is_whitespace(" \t\r\n\v:)"));
	TEST_ASSERT_EQUAL_INT(LSB(0b1101), 0);
}
