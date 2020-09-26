#include "protocols/support/pstate.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

static int
pcommand_cmp(const void *cmd1, const void *cmd2)
{
	const char *s1 = ((struct PCommand *)cmd1)->name;
	const char *s2 = ((struct PCommand *)cmd2)->name;
	return strcmp(s1, s2);
}

struct PState *
pstate_new(const char *str, const struct PCommand commands[], size_t count)
{
	struct PState *pstate = malloc(sizeof(struct PState));
	exit_if_null(pstate);
	char *str_w = exit_if_null(malloc(strlen(str) + 1));
	strcpy(str_w, str);
	*pstate = (struct PState){
		.original = str,
		.str = str_w,
		.token = NULL,
		.saveptr = NULL,
		.cmd = NULL,
	};
	pstate->token = strtok_r_whitespace(str_w, &pstate->saveptr);
	if (pstate->token) {
		struct PCommand key = {
			.name = pstate->token,
			.handler = NULL,
		};
		pstate->cmd = (struct PCommand *)(bsearch(
		  &key, commands, count, sizeof(struct PCommand), pcommand_cmp));
	}
	return pstate;
}

void
pstate_free(struct PState *pstate)
{
	free(pstate->str);
	free(pstate);
}

const char *
pstate_next(struct PState *pstate)
{
	return strtok_r_whitespace(NULL, &pstate->saveptr);
}

int
pstate_skip(struct PState *pstate, const char *expected)
{
	const char *str = pstate_next(pstate);
	if (!str) {
		return 0;
	} else if (strcmp(str, expected) == 0) {
		return 1;
	} else {
		return -1;
	}
}

const char *
pstate_next_sep(struct PState *pstate, const char *sep)
{
	if (!pstate->saveptr) {
		return NULL;
	}
	char *token = pstate->saveptr;
	pstate->saveptr = strstr(pstate->saveptr, sep);
	if (pstate->saveptr) {
		*pstate->saveptr = '\0';
		pstate->saveptr += strlen(sep);
	} else {
		pstate->saveptr = NULL;
	}
	return strtrim(token, WHITESPACE);
}

const char *
pstate_next_all(struct PState *pstate)
{
	if (!pstate->saveptr) {
		return NULL;
	}
	const char *str = pstate->saveptr + strspn(pstate->saveptr, WHITESPACE);
	pstate->saveptr = NULL;
	if (!*str) {
		return NULL;
	}
	return strtrim(str, WHITESPACE);
}
