#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "saves_dir.h"
#include "globals.h"
#include "utils.h"

#define SAVES_DIR_BUFFER_EXTRA_SIZE 80

struct SavesDir {
	size_t prefix_length;
	char buffer[];
};

struct SavesDir *
saves_dir_new(void) {
	char *path = getenv("Z64C_DIR");
	bool path_is_home_dir = !path;
	if (path_is_home_dir) {
		path = util_home_dir();
	}
	size_t path_length = strlen(path);
	struct SavesDir *saves_dir = malloc(sizeof(struct SavesDir) +
			                            path_length +
										SAVES_DIR_BUFFER_EXTRA_SIZE);
	strncpy(saves_dir->buffer, path, path_length);
	if (path_is_home_dir) {
		strcat(saves_dir->buffer, "/.Z64C");
		saves_dir->prefix_length = path_length + strlen("/.Z64C");
	} else {
		saves_dir->prefix_length = path_length;
	}
	return saves_dir;
}

void
saves_dir_drop(struct SavesDir *saves_dir) {
	free(saves_dir);
}

char *
saves_dir_ucirc_path(struct SavesDir *saves_dir) {
	sprintf(saves_dir->buffer + saves_dir->prefix_length, "/ucirc");
	return saves_dir->buffer;
}

char *
saves_dir_graph_path(struct SavesDir *saves_dir) {
	sprintf(saves_dir->buffer + saves_dir->prefix_length, "/graph");
	return saves_dir->buffer;
}

char *
saves_dir_help_path(struct SavesDir *saves_dir) {
	sprintf(saves_dir->buffer + saves_dir->prefix_length, "/help.txt");
	return saves_dir->buffer;
}

char *
saves_dir_session_path(struct SavesDir *saves_dir, size_t i) {
	sprintf(saves_dir->buffer + saves_dir->prefix_length, "/sessions/%zu", i);
	return saves_dir->buffer;
}
