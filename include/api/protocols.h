/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_API_PROTOCOLS_H
#define Z64C_API_PROTOCOLS_H

#include "engine.h"
#include "utils/dyn_str.h"

/* Z64C is multi-protocol and supports
 * - UCI (Tarrasch, Arena)
 * - UGEI
 *
 * A note regarding UGEI: I decided to develop a new standard because I felt
 * that all alternatives lacked something.
 *
 * @see https://xkcd.com/927/ */
void
engine_uci(struct Engine *engine, struct DynStr *dyn_str);
void
engine_ugei(struct Engine *engine, struct DynStr *dyn_str);
void
engine_unknown_protocol(struct Engine *engine, struct DynStr *dyn_str);

#endif
