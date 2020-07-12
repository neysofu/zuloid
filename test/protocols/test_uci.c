#include "chess/bb.h"
#include "engine.h"
#include "protocols/uci.h"

void
test_uci(void)
{
	struct Engine *engine = engine_new_tmp();
	// TEST_ASSERT_EQUAL_UINT(engine->status, STATUS_EXIT);
	engine_delete(engine);
}
