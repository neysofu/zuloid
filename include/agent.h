#pragma once

struct Agent;

struct Agent *
agent_new(void);

void
agent_delete(struct Agent *agent);
