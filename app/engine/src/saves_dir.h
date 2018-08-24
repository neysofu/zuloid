#pragma once

#include <stdint.h>

struct SavesDir;

struct SavesDir *
saves_dir_new(void);

void
saves_dir_drop(struct SavesDir *saves_dir);

char *
saves_dir_graph_path(struct SavesDir *saves_dir);

char *
saves_dir_help_path(struct SavesDir *saves_dir);

char *
saves_dir_session_path(struct SavesDir *saves_dir, size_t i);

char *
saves_dir_ucirc_path(struct SavesDir *saves_dir);
