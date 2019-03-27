/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_UTILS_H
#define Z64C_UTILS_H

#include "switches.h"
#include <stdbool.h>
#include <stdio.h>

/* Logs are always prefixed by '#', regardless of the protocol in use. This is
 * in line with both UGEI and CECP guidelines. UCI doesn't have a specific
 * syntax for debug statements, but simply ignores any unknown command (as # is). */
#ifdef NDEBUG
#define LOGF(...)
#else
#define LOGF(...)                                                                          \
	do {                                                                                   \
		/* Makes the path relative to the project root.                                    \
		 *                                ~~~~~~~~~~~~~~~~~~~~ */                          \
		printf("# %s:%s:%d -- ", __FILE__ + PROJECT_DIR_LENGTH, __func__, __LINE__);       \
		printf(__VA_ARGS__);                                                               \
		putchar('\n');                                                                     \
	} while (0)
#endif

void
log_unknown_command(const char *command);

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
	char *string;
	size_t capacity;
};

int
line_buffer_resize(struct LineBuffer *lb, size_t capacity);

extern const struct LineBuffer LINE_BUFFER_EMPTY;

/* Stores stream input in `lb` and waits for a line feed (ASCII 0xA). */
int
read_line(FILE *stream, struct LineBuffer *lb);

/* Writes the current process ID to `pid`.
 * @param pid a non-NULL target pointer.
 * @return 0 on success, -1 on failure.
 *
 * @see https://en.wikipedia.org/wiki/Process_identifier */
int
get_pid(int *pid);

#endif
