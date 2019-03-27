/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "cJSON/cJSON.h"
#include "engine.h"

#ifndef Z64C_METHODS_H
#define Z64C_METHODS_H

void
engine_ugei_call_config(struct Engine *engine, const cJSON *params, cJSON *response);
void
engine_ugei_call_get(const struct Engine *engine, const cJSON *params, cJSON *response);
void
engine_ugei_call_init(struct Engine *engine, const cJSON *params, cJSON *response);
void
engine_ugei_call_search(struct Engine *engine, const cJSON *params, cJSON *response);
void
engine_ugei_call_setup(struct Engine *engine, const cJSON *params, cJSON *response);
void
engine_ugei_call_status(struct Engine *engine, const cJSON *params, cJSON *response);
void
engine_ugei_call_stop(struct Engine *engine, const cJSON *params, cJSON *response);
void
engine_ugei_call_train(struct Engine *engine, const cJSON *params, cJSON *response);

void
engine_cecp_call_xboard(struct Engine *engine, struct Cmd *cmd);

#endif
