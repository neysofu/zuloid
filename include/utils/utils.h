/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_UTILS_H
#define Z64C_UTILS_H

#include "switches.h"
#include <stdbool.h>
#include <stdio.h>

enum ErrorCode
{
	ERR_CODE_NONE,
	ERR_CODE_EOF,
	ERR_CODE_ALLOC,
	ERR_CODE_UNSUPPORTED,
	ERR_CODE_INVALID_FEN,
};

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

bool
string_is_whitespace(const char *string);

/* Writes the current process ID to `pid`.
 * @param pid a non-NULL target pointer.
 * @return 0 on success, -1 on failure.
 *
 * @see https://en.wikipedia.org/wiki/Process_identifier */
int
get_pid(int *pid);

#endif
