#include "engine.h"
#include "munit/munit.h"
#include "protocols/uci.h"
#include "test/utils.h"
#include "utils.h"

void
test_perft_results(struct Engine *engine)
{
	char cmd[] = "isready";
	protocol_uci_handle(engine, cmd);
	struct Lines *lines = read_last_lines(engine->output);
	munit_assert_not_null(lines_nth(lines, -1));
	munit_assert_string_equal(lines_nth(lines, -1), "readyok");
}
