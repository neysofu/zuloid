/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "utils.h"
#include "UI/jsonrpc_errors.h"
#include "globals.h"
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#include <unistd.h>
#elif defined(_WIN32)
#include <processthreadsapi.h>
#endif

const char *const WHITESPACE_CHARS = "\t\n\r \v";

bool
string_is_whitespace(const char *string)
{
	assert(string);
	while (isspace(*string)) {
		string++;
	}
	return *string == '\0';
}

int
line_buffer_resize(struct LineBuffer *lb, size_t capacity)
{
	assert(lb);
	assert(capacity);
	char *new_string = realloc(lb->string, capacity);
	if (new_string) {
		memcpy(new_string, lb->string, lb->capacity);
		lb->capacity = capacity;
		lb->string = new_string;
		return 0;
	} else {
		free(lb->string);
		lb->string = NULL;
		return -1;
	}
}

int
read_line_from_stream(FILE *stream, struct LineBuffer *lb)
{
	assert(stream);
	assert(lb);
	if (lb->capacity > LINE_BUFFER_DEFAULT_CAPACITY * 2) {
		if (line_buffer_resize(lb, LINE_BUFFER_DEFAULT_CAPACITY)) {
			return -1;
		}
	}
	size_t length = 0;
	char c = '\0';
	do {
		c = fgetc(stream);
		if (c == EOF) {
			return -2;
		}
		lb->string[length++] = c;
		if (length == lb->capacity) {
			if (line_buffer_resize(lb, lb->capacity * 2) == -1) {
				return -1;
			}
		}
	} while (c != '\n');
	lb->string[length] = '\0';
}

struct PID
get_pid(void)
{
	return (struct PID)
	{
#if defined(__unix__) || defined(_POSIX_VERSION)
		true, getpid(),
#elif defined(_WIN32)
		true, GetCurrentProcessId(),
#else
		false, 0,
#endif
	};
}
