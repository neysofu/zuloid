#include "engine.h"
#include "munit/munit.h"
#include "protocols/cecp.h"
#include "protocols/uci.h"

void
test_protocol_switch(struct Engine *engine)
{
	{
		munit_assert(engine->config.protocol == protocol_uci);
	}
	engine->config.protocol(engine, "xboard");
	{
		munit_assert(engine->config.protocol == protocol_cecp);
	}
	engine->config.protocol(engine, "uci");
	{
		munit_assert(engine->config.protocol == protocol_uci);
	}
}
