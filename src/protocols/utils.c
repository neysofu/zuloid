#include "protocols/utils.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

static int
command_cmp(const void *cmd1, const void *cmd2)
{
	const char *s1 = ((struct Command *)cmd1)->name;
	const char *s2 = ((struct Command *)cmd2)->name;
	return strcmp(s1, s2);
}

struct PState *
pstate_new(const char *str, const struct Command commands[], size_t count)
{
	struct PState *pstate = malloc(sizeof(struct PState));
	exit_if_null(pstate);
	char *str_w = exit_if_null(malloc(strlen(str) + 1));
	strcpy(str_w, str);
	*pstate = (struct PState){
		.str = str_w,
		.token = NULL,
		.saveptr = NULL,
		.cmd = NULL,
	};
	pstate->token = strtok_r_whitespace(str_w, &pstate->saveptr);
	if (pstate->token) {
		struct Command key = {
			.name = pstate->token,
			.handler = NULL,
		};
		pstate->cmd = (struct Command *)(bsearch(
		  &key, commands, count, sizeof(struct Command), command_cmp));

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

void
display_err_syntax(FILE *stream)
{
	fputs("[ERROR] Invalid syntax.\n", stream);
}

void
display_err_unspecified(FILE *stream)
{
	fputs("[ERROR] Unspecified error.\n", stream);
}

void
display_err_invalid_command(FILE *stream)
{
	fputs("[ERROR] Invalid command.\n", stream);
}
