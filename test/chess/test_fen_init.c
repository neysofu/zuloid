#include "Unity/src/unity.h"
#include "chess/fen.h"
#include "chess/position.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
test_fen_init(void)
{
	char *fen = fen_from_position(NULL, &POSITION_INIT, ' ');
	TEST_ASSERT_EQUAL_STRING("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
	                         fen);
	free(fen);
}
