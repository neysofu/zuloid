#include "chess/bb.h"
#include "chess/fen.h"
#include "chess/movegen.h"
#include "chess/position.h"
#include "engine.h"
#include "munit/munit.h"
#include "protocols/uci.h"
#include "test/utils.h"
#include "utils.h"

#define POSITION_1 "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define POSITION_2 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - "

struct PerftTestCase
{
	const char *fen;
	int depth;
	uint64_t expected_result;
};

const struct PerftTestCase TEST_CASES[] = {
	{ POSITION_1, 0, 1 },      { POSITION_1, 1, 20 },     { POSITION_1, 2, 400 },
	{ POSITION_1, 3, 8902 },   { POSITION_1, 4, 197281 }, { POSITION_2, 1, 14 },
	{ POSITION_2, 2, 191 },    { POSITION_2, 3, 2812 },   { POSITION_2, 4, 43238 },
	{ POSITION_2, 5, 674624 },
};

const size_t TEST_CASES_COUNT = sizeof(TEST_CASES) / sizeof(TEST_CASES[0]);

void
test_perft_results(struct Engine *engine)
{
	bb_init();
	for (size_t i = 0; i < TEST_CASES_COUNT; i++) {
		char *fen = exit_if_null(malloc(FEN_SIZE));
		strcpy(fen, TEST_CASES[i].fen);
		position_init_from_fen(&engine->board, fen);
		uint64_t result =
		  position_perft(engine->output, &engine->board, TEST_CASES[i].depth);
		free(fen);
		{
			munit_assert_uint(result, ==, TEST_CASES[i].expected_result);
		}
	}
}
