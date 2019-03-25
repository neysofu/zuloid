/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "utils.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

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
	cmd->i = strspn(cmd->string, WHITESPACE_CHARS);
	for (size_t i = cmd->i; i < cmd->length; i++) {
		if (strchr(WHITESPACE_CHARS, string[i])) {
			string[i] = '\0';
		}
	}
}

char *
cmd_next(struct Cmd *cmd)
{
	assert(cmd);
	assert(cmd->string);
	cmd->i += strlen(cmd->string + cmd->i);
	while (cmd->i < cmd->length && cmd->string == '\0') {
		cmd->i++;
	}
	if (cmd->i == cmd->length) {
		cmd->i = 0;
		return NULL;
	} else {
		return cmd->string + cmd->i;
	}
}

char *
cmd_current(struct Cmd *cmd)
{
	assert(cmd);
	assert(cmd->string);
	assert(cmd->string[cmd->i]);
	return cmd->string + cmd->i;
}

char *
cmd_at(struct Cmd *cmd, size_t i)
{
	assert(cmd);
	assert(cmd->string);
	while (i-- > 0) {
		cmd_next(cmd);
	}
	return cmd->string + cmd->i;
}
