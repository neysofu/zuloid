#ifndef ZORRO_AGENT_H
#define ZORRO_AGENT_H

struct Agent;

struct Agent *
agent_new(void);

void
agent_delete(struct Agent *agent);

#endif
