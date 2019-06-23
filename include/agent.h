/* Copyright (c) Filippo Costa - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * PROPRIETARY AND CONFIDENTIAL */

#ifndef Z64C_AGENT_H
#define Z64C_AGENT_H

struct Agent;

struct Agent *
agent_new(void);

void
agent_delete(struct Agent *agent);

#endif
