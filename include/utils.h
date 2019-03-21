/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_UTILS_H
#define Z64C_UTILS_H

#include "switches.h"
#include <stdbool.h>
#include <stdio.h>

#ifdef NDEBUG
#define LOGF(...)
#else
#define LOGF(...)                                                                          \
	do {                                                                                   \
		printf("# %s:%s:%d -- ", &__FILE__[PROJECT_DIR_LENGTH], __func__, __LINE__);       \
		printf(__VA_ARGS__);                                                               \
		putchar('\n');                                                                     \
	} while (0)
#endif

#define UNUSED(...) (void)(__VA_ARGS__)

extern const char *const WHITESPACE_CHARS;

bool
string_is_whitespace(const char *string);

enum
{
	LINE_BUFFER_DEFAULT_CAPACITY = 64,
};

struct LineBuffer
{
	size_t capacity;
	char *string;
};

int
line_buffer_resize(struct LineBuffer *lb, size_t capacity);

int
read_line_from_stream(FILE *stream, struct LineBuffer *lb);

struct PID
{
	bool success;
	int value;
};

struct PID
get_pid(void);

#endif
