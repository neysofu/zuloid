/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_UTILS_H
#define Z64C_UTILS_H

#include <stdbool.h>
#include <stdio.h>

void
logg(const char *format, ...);

void *
exit_if_null(void *ptr);

void *
malloc_or_exit(size_t size);

bool
string_is_whitespace(const char *string);

char *
read_line_from_stream(FILE *stream);

struct PID
{
	int value;
	bool success;
};

struct PID
get_pid(void);

#endif
