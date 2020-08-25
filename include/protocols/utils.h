#ifndef ZULOID_PROTOCOLS_UTILS_H
#define ZULOID_PROTOCOLS_UTILS_H

#include "engine.h"
#include <stdlib.h>

struct PState;

struct PCommand
{
	const char *name;
	void (*handler)(struct Engine *, struct PState *);
};

struct PState {
	char *str;
	char *token;
	char *saveptr;
	const struct PCommand *cmd;
};

struct PState *
pstate_new(const char *str, const struct PCommand commands[], size_t count);

void
pstate_free(struct PState *pstate);

const char *
pstate_next(struct PState *pstate);

void
display_err_syntax(FILE *stream);
void
display_err_unspecified(FILE *stream);
void
display_err_invalid_command(FILE *stream);

#endif
