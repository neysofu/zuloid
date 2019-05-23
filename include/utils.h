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

/* Writes the current process ID to `pid`.
 * @param pid a non-NULL target pointer.
 * @return 0 on success, -1 on failure.
 *
 * @see https://en.wikipedia.org/wiki/Process_identifier */
int
get_pid(int *pid);

long
getline(char **buf, size_t *bufsiz, FILE *fp);

char *
strsep_whitespace(char **stringp);

#endif
