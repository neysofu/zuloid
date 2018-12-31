/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "utils.h"
#include "globals.h"
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#include <unistd.h>
#elif defined(_WIN32)
#include <processthreadsapi.h>
#endif

bool
string_is_comment_or_whitespace(const char *str)
{
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
	exit(1);
}

int
get_pid(void)
{
#if defined(__unix__) || defined(_POSIX_VERSION)
	return getpid();
#elif defined(_WIN32)
	return GetCurrentProcessId();
#else
	return -1;
#endif
}

int
get_architecture_bits(void)
{
	return sizeof(void *) * CHAR_BIT;
}

void
print_welcome_message(void)
{
	printf("# .:.:. Welcome to Z64C .:.:.\r\n");
	printf("# version = %s, nr. bits = %lu, release date = %s",
	       Z64C_VERSION,
	       ARCHITECTURE_BITS,
	       Z64C_RELEASE_DATE_ISO_8601);
	if (get_pid() != -1) {
		printf(", PID = %d", get_pid());
	}
	printf("\r\n");
}

char *
read_line_from_stream(FILE *stream)
{
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
