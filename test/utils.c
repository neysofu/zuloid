/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "utils.h"
#include "Unity/src/unity.h"

void
test_utils(void)
{
	TEST_ASSERT(string_is_whitespace(""));
	TEST_ASSERT(string_is_whitespace("\n"));
	/* Regression test. */
	TEST_ASSERT_FALSE(string_is_whitespace("# foobar"));
	TEST_ASSERT_FALSE(string_is_whitespace("\t  ##  # "));
}
