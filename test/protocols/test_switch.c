#include "engine.h"
#include "munit/munit.h"
#include "protocols/protocols.h"

void
test_protocol_switch(struct Engine *engine)
{
	{
		munit_assert_ptr_equal(engine->protocol, protocol_uci);
	}
	engine->protocol(engine, "xboard");
	{
		munit_assert_ptr_equal(engine->protocol, protocol_cecp);
	}
	engine->protocol(engine, "uci");
	{
		munit_assert_ptr_equal(engine->protocol, protocol_uci);
	}
}
