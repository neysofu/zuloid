#ifndef ZORRO_PROTOCOLS_H
#define ZORRO_PROTOCOLS_H

#include "engine.h"

void
protocol_cecp(struct Engine *restrict engine, const char *cmd);
void
protocol_uci(struct Engine *restrict engine, const char *cmd);

#endif
