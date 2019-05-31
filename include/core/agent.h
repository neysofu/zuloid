/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_CORE_AGENT_H
#define Z64C_CORE_AGENT_H

#include "utils.h"
#include <stdio.h>

/* Our AI agent. Its responsabilities are:
 * - position evaluation
 * - searching algorith */
struct Agent;

struct Agent *
agent_new(void);

enum ErrorCode
agent_stop(struct Agent *agent);

void
agent_delete(struct Agent *agent);

int
agent_import(struct Agent *agent, FILE *file);

int
agent_export(struct Agent *agent, FILE *file);

#endif
