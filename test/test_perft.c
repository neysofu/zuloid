#include "engine.h"
#include "protocols/uci.h"

void
test_perft_results(struct Engine *engine)
{
	protocol_uci_handle(engine, "go perft 1");
}
