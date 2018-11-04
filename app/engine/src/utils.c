#include "utils.h"
#include <pwd.h>
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

void *
xmalloc(size_t size)
{
	void *ptr = malloc(size);
	if (UNLIKELY(!ptr)) {
		exit(EX_OSERR);
	}
	return ptr;
}
