/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "utils.h"
#include "globals.h"
#include "switches.h"
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

void *
exit_if_null(void *ptr)
{
	if (ptr) {
		return ptr;
	} else {
		puts(
		  "\t{\"jsonrpc\":\"2.0\",\"id\":null,\"error\":{\"code\":21,\"message\":\"Out-of-"
		  "memory condition\"}}");
		exit(EXIT_FAILURE);
	}
}

void *
malloc_or_exit(size_t size)
{
	return exit_if_null(malloc(size));
}

bool
string_is_whitespace(const char *string)
{
	assert(string);
	while (isspace(*string)) {
		string++;
	}
	return '\0' == *string;
}

char *
read_line_from_stream(FILE *stream)
{
	assert(stream);
	size_t str_length = 0;
	size_t str_max_length = 64;
	char *str = malloc_or_exit(str_max_length);
	char c;
	do {
		c = fgetc(stdin);
		if (c == EOF) {
			free(str);
			return NULL;
		}
		str[str_length++] = c;
		if (str_length == str_max_length) {
			str = exit_if_null(realloc(str, (str_max_length *= 2)));
		}
	} while (c != '\n');
	str[str_length] = '\0';
	return str;
}

struct PID
get_pid(void)
{
	return (struct PID)
	{
#if defined(__unix__) || defined(_POSIX_VERSION)
		getpid(), true,
#elif defined(_WIN32)
		GetCurrentProcessId(), true,
#else
		0, false,
#endif
	};
}
