/* Copyright (c) Filippo Costa - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * PROPRIETARY AND CONFIDENTIAL */

#include "Unity/src/unity.h"
#include "chess/bb.h"
#include "chess/movegen.h"

void
test_attacks(void)
{
	TEST_ASSERT_EQUAL_INT64(0x1010101010101FEULL, bb_bishop(0, 0));
	TEST_ASSERT_EQUAL_INT64(0x7E01010100000000ULL, bb_bishop(56, 0x8000041001300001ULL));
}
