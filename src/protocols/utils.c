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

const struct Command *
identify_command(const char *token, const struct Command commands[], size_t count)
{
	const struct Command key = {
		.name = token,
		.handler = NULL,
	};
	return bsearch(&key, commands, count, sizeof(struct Command), command_cmp);
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
