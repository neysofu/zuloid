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

void
log_unknown_command(const char *command)
{
	printf("# Unknown command: '%s'.\n", command);
}

bool
string_is_whitespace(const char *string)
{
	assert(string);
	while (isspace(*string)) {
		string++;
	}
	return *string == '\0';
}

const struct LineBuffer LINE_BUFFER_EMPTY = {
	.string = NULL,
	.capacity = 0,
};

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
read_line(FILE *stream, struct LineBuffer *lb)
{
	assert(stream);
	assert(lb);
	if (lb->capacity > LINE_BUFFER_DEFAULT_CAPACITY * 2) {
		if (line_buffer_resize(lb, LINE_BUFFER_DEFAULT_CAPACITY) == EXIT_FAILURE) {
			return EXIT_FAILURE;
		}
	}
	size_t length = 0;
	char c = '\0';
	do {
		c = fgetc(stream);
		if (c == EOF) {
			return EXIT_FAILURE;
		}
		lb->string[length++] = c;
		if (length == lb->capacity) {
			if (line_buffer_resize(lb, lb->capacity * 2) == EXIT_FAILURE) {
				return EXIT_FAILURE;
			}
		}
	} while (c != '\n');
	lb->string[length] = '\0';
	return EXIT_SUCCESS;
}

int
get_pid(int *pid)
{
	assert(pid);
#if defined(__unix__) || defined(_POSIX_VERSION)
	*pid = getpid();
	return EXIT_SUCCESS;
#elif defined(_WIN32)
	*pid = GetCurrentProcessId();
	return EXIT_SUCCESS;
#else
	return EXIT_FAILURE;
#endif
}
