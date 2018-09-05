#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <pwd.h>

bool
util_is_stream_empty(FILE *stream) {
    fd_set read_fs;
    FD_ZERO(&read_fs);
    FD_SET(fileno(stream), &read_fs);
    return select(fileno(stream) + 1, &read_fs, NULL, NULL, NULL);
}

char *
util_home_dir(void) {
	char *path = getenv("HOME");
	if (!path) {
		path = getpwuid(getuid())->pw_dir;
	}
	return path;
}

size_t
util_timestamp_msec(void) {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

size_t
util_page_size(void) {
	return sysconf(_SC_PAGESIZE);
}
