#include "chess/fen.h"
#include "chess/position.h"
#include "munit/munit.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
test_fen_init(void)
{
	char *fen = fen_from_position(NULL, &POSITION_INIT, ' ');
	munit_assert_string_equal("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
	                          fen);
	free(fen);
}
