#include "engine.h"
#include "munit/munit.h"
#include "protocols/uci.h"
#include "test/utils.h"
#include "utils.h"

void
test_uci_unknown_cmd(struct Engine *engine)
{
	char cmd[] = "foobar";
	protocol_uci_handle(engine, cmd);
	struct Lines *lines = read_last_lines(engine->output);
	munit_assert_uint(lines_count(lines), ==, 1);
	munit_assert_true(strstr(lines_nth(lines, 0), "ERROR") != NULL);
	lines_delete(lines);
}
