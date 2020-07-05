#ifndef ZORRO_PROTOCOLS_H
#define ZORRO_PROTOCOLS_H

#include "engine.h"

void
engine_uci(struct Engine *engine, char *cmd);
void
engine_unknown_protocol(struct Engine *engine, char *cmd);

#endif
