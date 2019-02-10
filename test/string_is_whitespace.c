/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "utils.h"
#include "Unity/src/unity.h"
#include <stdlib.h>

void
test_string_is_whitespace_empty_string(void)
{
	TEST_ASSERT(string_is_whitespace(""));
}

void
test_string_is_whitespace_many_kinds_of_whitespace(void)
{
	TEST_ASSERT(string_is_whitespace(" \v\t\r\n"));
}

void
test_string_is_whitespace_partially_whitespace(void)
{
	TEST_ASSERT_FALSE(string_is_whitespace(" some spam"));
}

void
test_string_is_whitespace_final_non_whitespace_character(void)
{
	TEST_ASSERT_FALSE(string_is_whitespace(" ."));
}

void
test_string_is_whitespace_number_sign(void)
{
	TEST_ASSERT_FALSE(string_is_whitespace("#"));
}
