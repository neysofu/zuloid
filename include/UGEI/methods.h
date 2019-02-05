/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "cJSON/cJSON.h"
#include "engine.h"

#ifndef Z64C_UGEI_METHODS_H
#define Z64C_UGEI_METHODS_H

void
engine_call_config(struct Engine *engine, const struct cJSON *params, struct cJSON *response);

void
engine_call_get(const struct Engine *engine, const struct cJSON *params, struct cJSON *response);

void
engine_call_init(struct Engine *engine, const struct cJSON *params, struct cJSON *response);

void
engine_call_search(struct Engine *engine, const struct cJSON *parms, struct cJSON *response);

void
engine_call_setup(struct Engine *engine, const struct cJSON *parms, struct cJSON *response);

#endif
