/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_CMD_H
#define Z64C_CMD_H

#include <stdlib.h>

struct Cmd
{
	char *string;
	size_t i;
};

/* There is no memory allocation under the hood, so no need for 'engine_delete'. */
void
cmd_init(struct Cmd *cmd, char *string);

/* Parses the command string and returns a pointer to the next token,
 * null-terminated.
 *
 * If there are no more tokens, it returns NULL.
 *
 * RETURNS: a pointer to the next token. */
char *
cmd_next(struct Cmd *cmd);

/* Brings the iterator to the nth token and returns it. */
char *
cmd_at(struct Cmd *cmd, size_t i);

#endif
