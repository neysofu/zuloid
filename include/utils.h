/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_UTILS_H
#define Z64C_UTILS_H

#include <stdio.h>

/* Error codes to be used all around the codebase when necessary. */
enum ErrorCode
{
	ERR_CODE_NONE,
	ERR_CODE_EOF,
	ERR_CODE_ALLOC,
	ERR_CODE_UNSUPPORTED,
	ERR_CODE_INVALID_FEN,
};

/* It reads an entire line from `stream`, similarly from POSIX's getline. */
long
read_line(char **lineptr, size_t *n, FILE *stream);

/* TODO: make it thread-safe. */
char *
strtok_whitespace(char *str);

#endif
