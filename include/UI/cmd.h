/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_CMD_H
#define Z64C_CMD_H

struct Cmd
{
	char *string;
	size_t i;
};

struct Cmd
cmd_new(char *string);

char *
cmd_next(struct Cmd *cmd);

char *
cmd_at(struct Cmd *cmd, size_t i);

#endif
