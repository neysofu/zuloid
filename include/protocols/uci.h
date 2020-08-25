#ifndef ZULOID_PROTOCOLS_UCI_H
#define ZULOID_PROTOCOLS_UCI_H

#include "engine.h"

void
engine_call_uci(struct Engine *restrict engine, const char *cmd);

#endif
