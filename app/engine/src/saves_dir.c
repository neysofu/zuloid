#include "saves_dir.h"
#include "globals.h"

#define SAVES_DIR_BUFFER_EXTRA_SIZE 80

struct SavesDir {
	char *path;
	char buffer[];
};

struct SavesDir *
saves_dir_new(void) {
	struct SavesDir *saves_dir = malloc(sizeof(struct SavesDir) +
			                            strlen(Z64C_DIR) +
										SAVES_DIR_BUFFER_EXTRA_SIZE);
}

void
saves_dir_drop(struct SavesDir *saves_dir) {
	free(saves_dir);
}

char *
saves_dir_ucirc_path(struct SavesDir *saves_dir) {
	return sprintf(saves_dir->buffer, "%s/ucirc", saves_dir->path);
}

char *
saves_dir_graph_path(struct SavesDir *saves_dir) {
	return sprintf(saves_dir->buffer, "%s/graph", saves_dir->path);
}

char *
saves_dir_help_path(struct SavesDir *saves_dir) {
	return sprintf(saves_dir->buffer, "%s/help.txt", saves_dir->path);
}

char *
saves_dir_session_path(struct SavesDir *saves_dir, size_t i) {
	return sprintf(saves_dir->buffer, "%s/sessions/%llu", saves_dir->path, i);
}
