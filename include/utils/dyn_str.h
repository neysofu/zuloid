/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_UTILS_DYN_STR_H
#define Z64C_UTILS_DYN_STR_H

#include <stdio.h>
#include <stdlib.h>

struct DynStr
{
	char *buffer;
	size_t length;
	size_t capacity;
	size_t i;
};

void
dyn_str_init(struct DynStr *dyn_str, char *string);

int
dyn_str_read_line_from_stream(struct DynStr *dyn_str, FILE *stream);

void
dyn_str_tokenize(struct DynStr *dyn_str);

/* Parses the command string and returns a pointer to the current token,
 * null-terminated.
 *
 * If there are no more tokens, it returns NULL.
 *
 * RETURNS: a pointer to the next token. */
char *
dyn_str_current_token(struct DynStr *dyn_str);

/* Parses the command string and returns a pointer to the next token,
 * null-terminated.
 *
 * If there are no more tokens, it returns NULL.
 *
 * RETURNS: a pointer to the next token. */
char *
dyn_str_next_token(struct DynStr *dyn_str);

const struct DynStr DYN_STR_EMPTY;

#endif
