#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

bool
atob(char *str) {
	if (strcmp(str, "yes") == 0) {
		return true;
	} else if (strcmp(str, "true")) {
		return true;
	} else if (strcmp(str, "no")) {
		return false;
	} else if (strcmp(str, "false")) {
		return false;
	} else {
		return false;
	}
}

bool
is_stdin_empty(void) {
    fd_set read_fs;
    FD_ZERO(&read_fs);
    FD_SET(fileno(stdin), &read_fs);
    return select(fileno(stdin) + 1, &read_fs, NULL, NULL, NULL);
}
