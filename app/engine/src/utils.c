#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <pwd.h>

bool
is_stdin_empty(void) {
    fd_set read_fs;
    FD_ZERO(&read_fs);
    FD_SET(fileno(stdin), &read_fs);
    return select(fileno(stdin) + 1, &read_fs, NULL, NULL, NULL);
}

char *
home_dir(void) {
	static char *home_dir = getenv("HOME");
	if (!home_dir) {
		home_dir = getpwuid(getuid())->pw_dir;
	}
	return home_dir;
}

size_t
timestamp_msec(void) {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

size_t
page_size(void) {
	return sysconf(_SC_PAGESIZE);
}
