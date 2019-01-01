/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_UTILS_H
#define Z64C_UTILS_H

#include "switches.h"
#include <stdbool.h>
#include <stdio.h>

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

#define LOG(fmt, args...)                                                                  \
	do {                                                                                   \
		if (SWITCH_LOGGING) {                                                              \
			printf("# %s:%s:%d -- " fmt "\r\n",                                            \
			       __FILE__ + SOURCE_PATH_LENGTH,                                          \
			       __func__,                                                               \
			       __LINE__,                                                               \
			       ##args);                                                                \
		}                                                                                  \
	} while (0)

void *
handle_oom(void *ptr);

bool
string_is_comment_or_whitespace(const char *str);

struct PID
{
	int value;
	bool success;
};

struct PID
get_pid(void);

char *
read_line_from_stream(FILE *stream);

#endif
