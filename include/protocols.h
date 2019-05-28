/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_PROTOCOLS_H
#define Z64C_PROTOCOLS_H

#include "engine.h"

void
engine_uci(struct Engine *engine, char *cmd);
void
engine_unknown_protocol(struct Engine *engine, char *cmd);

#endif
