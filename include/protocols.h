#ifndef ZULOID_PROTOCOLS_H
#define ZULOID_PROTOCOLS_H

#include "engine.h"

void
engine_uci(struct Engine *restrict engine, char *cmd);
void
engine_unknown_protocol(struct Engine *restrict engine, char *cmd);

#endif
