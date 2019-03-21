/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "utils.h"
#include <assert.h>
#include <stdlib.h>

struct Cmd
{
	char *string;
	size_t length;
	size_t i;
};

void
cmd_init(struct Cmd *cmd, char *string)
{
	assert(cmd);
	assert(string);
	cmd->string = string;
	cmd->length = strlen(string);
	cmd->i = 0;
}

char *
cmd_next(struct Cmd *cmd)
{
	assert(cmd);
	assert(cmd->string);
	cmd->i += strcspn(cmd->string[cmd->i], WHITESPACE_CHARS);
	cmd->i += strspn(cmd->string[cmd->i], WHITESPACE_CHARS);
	cmd->string[cmd->i - 1] = '\0';
	return cmd->string[cmd->i];
}

char *
cmd_at(struct Cmd *cmd, size_t i)
{
	assert(cmd);
	assert(cmd->string);
	return cmd->string;
}
