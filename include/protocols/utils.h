#ifndef ZORRO_PROTOCOLS_UTILS_H
#define ZORRO_PROTOCOLS_UTILS_H

#include "engine.h"
#include <stdlib.h>

struct Command
{
	const char *name;
	void (*handler)(struct Engine *);
};

const struct Command *
identify_command(const char *token, const struct Command commands[], size_t count);

int
command_cmp(const void *cmd1, const void *cmd2);

void
display_err_syntax(FILE *stream);
void
display_err_unspecified(FILE *stream);
void
display_err_invalid_command(FILE *stream);

#endif
