/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "Unity/src/unity.h"
#include "utils/dyn_str.h"
#include <string.h>

void
test_dyn_str(void)
{
	struct DynStr dyn_str;
	dyn_str_init(&dyn_str, "");
	dyn_str_tokenize(&dyn_str);
	TEST_ASSERT(strcmp(dyn_str_current_token(&dyn_str), "") == 0);
	TEST_ASSERT(strcmp(dyn_str_next_token(&dyn_str), "") == 0);
	TEST_ASSERT(strcmp(dyn_str_current_token(&dyn_str), "") == 0);
	TEST_ASSERT(strcmp(dyn_str_next_token(&dyn_str), "") == 0);
}
