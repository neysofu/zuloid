/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "Unity/src/unity.h"
#include "chess/board.h"
#include "chess/move.h"
#include "cJSON/cJSON.h"

void
test_perft(struct Engine *engine, char *fen, size_t depth, size_t expected_count)
{
	struct cJSON *response = cJSON_Parse(engine_rpc(engine, fen));
	TEST_ASSERT(response);
	TEST_ASSERT_FALSE(cJSON_GetErrorPtr());
	struct cJSON *result = cJSON_GetObjectItem(response, "result");
	size_t count = cJSON_GetObjectItem(result, "count")->valuedouble;
	TEST_ASSERT_EQUAL_INT(count, expected_count);
}

void
test_perft_all(void)
{
//	struct Engine *engine = engine_new();
//	test_perft(engine, 0, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 0);
//	test_perft(engine, 1, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 20);
//	test_perft(engine, 2, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 5);
//	test_perft(engine, 3, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 5);
//	test_perft(engine, 4, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 5);
//	test_perft(engine, 5, "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1", 3797944);
//	engine_free(engine);
}
