#include "engine.h"
#include "munit/munit.h"
#include "protocols/cecp.h"
#include "test/utils.h"
#include "utils.h"

void
test_protocol_cecp(struct Engine *engine)
{
	protocol_cecp(engine, "xboard");
	protocol_cecp(engine, "protover 2");
	{
		struct Lines *lines = file_line_by_line(engine->output);
		for (size_t i = 0; i < lines_count(lines); i++) {
			munit_assert_not_null(strstr(lines_nth(lines, i), "feature"));
		}
		lines_delete(lines);
	}
}

void
test_protocol_cecp_ping(struct Engine *engine)
{
	protocol_cecp(engine, "ping 0");
	protocol_cecp(engine, "ping -1");
	protocol_cecp(engine, "ping 128");
	protocol_cecp(engine, "ping -1025");
	{
		struct Lines *lines = file_line_by_line(engine->output);
		munit_assert_string_equal(lines_nth(lines, 0), "pong 0");
		munit_assert_string_equal(lines_nth(lines, 1), "pong -1");
		munit_assert_string_equal(lines_nth(lines, 2), "pong 128");
		munit_assert_string_equal(lines_nth(lines, 3), "pong -1025");
		lines_delete(lines);
	}
}

void
test_protocol_cecp_quit(struct Engine *engine)
{
	{
		munit_assert_uint(engine->status, !=, STATUS_EXIT);
	}
	protocol_cecp(engine, "quit");
	{
		munit_assert_uint(engine->status, ==, STATUS_EXIT);
	}
}
