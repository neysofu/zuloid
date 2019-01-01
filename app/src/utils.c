/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "utils.h"
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

void
logg(const char *format, ...)
{
#ifdef SWITCH_LOGGING
	va_list args;
	va_start(args, format);
	fprintf(stderr, "# %s:%s:%d -- ", __FILE__ + SOURCE_PATH_LENGTH, __func__, __LINE__);
	vfprintf(stderr, format, args);
	fprintf(stderr, "\r\n");
	va_end(args);
#endif
}

bool
string_is_comment_or_whitespace(const char *str)
{
	assert(str);
	while (isspace(*str)) {
		str++;
	}
	return !*str || *str == '#';
}

void *
handle_oom(void *ptr)
{
	if (ptr) {
		return ptr;
	}
	printf("{\"jsonrpc\":\"2.0\",\"id\":null,\"error\":{\"code\":900,\"message\":\"OOM "
	       "error\"}}\r\n");
	exit(EXIT_FAILURE);
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

char *
read_line_from_stream(FILE *stream)
{
	assert(stream);
	size_t str_length = 0;
	size_t str_max_length = 64;
	char *str = malloc(str_max_length);
	if (!str) {
		return NULL;
	}
	char c;
	while ((c = fgetc(stdin)) != EOF) {
		str[str_length++] = c;
		if (str_length == str_max_length) {
			char *new_str = realloc(str, (str_max_length *= 2));
			if (!new_str) {
				free(str);
				return NULL;
			}
			str = new_str;
		}
		if (c == '\n') {
			break;
		}
	}
	str[str_length] = '\0';
	return str;
}
