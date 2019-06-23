/* Copyright (c) Filippo Costa - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * PROPRIETARY AND CONFIDENTIAL */

#ifndef Z64C_UTILS_H
#define Z64C_UTILS_H

#include <stdio.h>

/* Catch-all error codes for the whole codebase. Add new cases as you please. You should
 * probably restrain from showing it to the end user though. */
enum ErrorCode
{
	ERR_CODE_NONE,
	ERR_CODE_EOF,
	ERR_CODE_ALLOC,
	ERR_CODE_UNSUPPORTED,
	ERR_CODE_INVALID_FEN,
};

/* It reads an entire line from 'stream', similar to POSIX's 'getline'. */
long
read_line(char **lineptr, size_t *n, FILE *stream);

/* TODO: make it thread-safe. */
char *
strtok_whitespace(char *str);

#endif
