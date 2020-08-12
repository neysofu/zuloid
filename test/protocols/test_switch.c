#include "engine.h"
#include "munit/munit.h"
#include "protocols/cecp.h"
#include "protocols/uci.h"

void
test_protocol_switch(struct Engine *engine)
{
	{
		munit_assert(engine->protocol == protocol_uci);
	}
	engine->protocol(engine, "xboard");
	{
		munit_assert(engine->protocol == protocol_cecp);
	}
	engine->protocol(engine, "uci");
	{
		munit_assert(engine->protocol == protocol_uci);
	}
}
