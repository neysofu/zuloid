#ifndef ZORRO_PROTOCOLS_UCI_H
#define ZORRO_PROTOCOLS_UCI_H

#include "engine.h"

struct UciCommand
{
	const char *name;
	void (*handler)(struct Engine *);
};

extern const struct UciCommand UCI_COMMANDS[];
extern const size_t UCI_COMMANDS_COUNT;

extern const char *UCI_OPTIONS[];
extern const size_t UCI_OPTIONS_COUNT;

int
uci_command_cmp(const void *cmd1, const void *cmd2);

void
protocol_uci_handle(struct Engine *restrict engine, const char *cmd);

#endif
