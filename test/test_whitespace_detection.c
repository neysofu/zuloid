/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "Unity/src/unity.h"
#include <stdlib.h>

void
test_whitespace_detection(void)
{
	TEST_ASSERT(string_is_whitespace(""));
	TEST_ASSERT(string_is_whitespace(" \v\t\r\n"));
	TEST_ASSERT_FALSE(string_is_whitespace(" some spam"));
	TEST_ASSERT_FALSE(string_is_whitespace(" ."));
	TEST_ASSERT_FALSE(string_is_whitespace("#"));
}
