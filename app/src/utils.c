/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "utils.h"
#include "debug.h"
#include "rpc.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sysexits.h>
#include <unistd.h>

bool
util_is_stream_empty(FILE *stream)
{
	fd_set read_fs;
	FD_ZERO(&read_fs);
	FD_SET(fileno(stream), &read_fs);
	return select(fileno(stream) + 1, &read_fs, NULL, NULL, NULL);
}

size_t
util_timestamp_msec(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

bool
util_str_is_whitespace(const char *str)
{
	assert(str);
	while (isspace(*str) && str++)
		;
	return !*str;
}

void *
xmalloc(size_t size)
{
	void *ptr = malloc(size);
	if (UNLIKELY(!ptr)) {
		printf("\n{\"jsonrpc\":\"2.0\",\"id\":null,\"error\":{\"code\":%d,\"error\":\"%s\"}"
		       "}\r\n",
		       JSONRPC_OOM_CODE,
		       JSONRPC_OOM_MSG);
		exit(EX_OSERR);
	}
	return ptr;
}
