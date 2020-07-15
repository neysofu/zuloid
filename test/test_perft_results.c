#include "engine.h"
#include "munit/munit.h"
#include "protocols/uci.h"
#include "test/utils.h"
#include "chess/movegen.h"
#include "chess/bb.h"
#include "chess/position.h"
#include "utils.h"

void
test_perft_results(struct Engine *engine)
{
	bb_init();
	struct Board initial_position = POSITION_INIT;
	munit_assert_uint(position_perft(engine->output, &initial_position, 1), ==, 20);
	munit_assert_uint(position_perft(engine->output, &initial_position, 2), ==, 400);
	munit_assert_uint(position_perft(engine->output, &initial_position, 3), ==, 8902);
	munit_assert_uint(position_perft(engine->output, &initial_position, 4), ==, 197281);
}
