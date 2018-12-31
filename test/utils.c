/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "utils.h"
#include "Unity/src/unity.h"

void
test_utils(void)
{
	TEST_ASSERT_EQUAL_INT(1337, MAX(42, 1337));
	TEST_ASSERT_EQUAL_INT(1337, MAX(1337, 42));
	TEST_ASSERT_EQUAL_INT(-1, MIN(-1, 0));
	TEST_ASSERT_EQUAL_INT(3, MIN(3, 3));
	TEST_ASSERT(string_is_comment_or_whitespace(""));
	TEST_ASSERT(string_is_comment_or_whitespace("\t  ##spam"));
	TEST_ASSERT(string_is_comment_or_whitespace("# foobar"));
	TEST_ASSERT(string_is_comment_or_whitespace("\n"));
}
