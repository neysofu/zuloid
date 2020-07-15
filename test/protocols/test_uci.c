#include "engine.h"
#include "munit/munit.h"
#include "protocols/uci.h"
#include "test/utils.h"
#include "utils.h"

void
test_uci(struct Engine *engine)
{
	char cmd[] = "isredy";
	protocol_uci_handle(engine, cmd);
	//struct Lines *lines = read_last_lines(engine->output, 1);
	//munit_assert_string_equal(lines_nth(lines, -1), "readyok");
}
