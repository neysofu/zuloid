#include "protocols/protocols.h"
#include "core.h"
#include "engine.h"
#include "utils.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

void
protocol_handshake(struct Engine *engine, const char *s)
{
	if (streq(s, "uci")) {
		engine->protocol = protocol_uci;
	} else if (streq(s, "xboard")) {
		engine->protocol = protocol_cecp;
	} else {
		return;
	}
	engine->protocol(engine, s);
}
