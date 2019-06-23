/* Copyright (c) Filippo Costa - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * PROPRIETARY AND CONFIDENTIAL */

#ifndef Z64C_PROTOCOLS_H
#define Z64C_PROTOCOLS_H

#include "engine.h"

void
engine_uci(struct Engine *engine, char *cmd);
void
engine_unknown_protocol(struct Engine *engine, char *cmd);

#endif
