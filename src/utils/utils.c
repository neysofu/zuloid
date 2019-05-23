/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "utils.h"
#include <assert.h>
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

#define BUFSIZE 32

void
log_unknown_command(const char *command)
{
	printf("# Unknown command: '%s'\n", command);
}

int
get_pid(int *pid)
{
	assert(pid);
#if defined(__unix__) || defined(_POSIX_VERSION)
	*pid = getpid();
	return ERR_CODE_NONE;
#elif defined(_WIN32)
	*pid = GetCurrentProcessId();
	return ERR_CODE_NONE;
#else
	return ERR_CODE_UNSUPPORTED;
#endif
}
