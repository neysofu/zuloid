/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chess/fen.h"
#include "Unity/src/unity.h"
#include "chess/position.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
test_fen_init(void)
{
	char *fen = fen_new_from_position(&POSITION_DEFAULT);
	TEST_ASSERT_EQUAL_STRING("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", fen);
	free(fen);
}
