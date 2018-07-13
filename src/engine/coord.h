#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "dir.h"

typedef uint8_t Rank;
typedef uint8_t File;

struct Coord {
	File file;
	Rank rank;
};

struct Coord
coord_new(File file, Rank rank);

char *
coord_to_str(const struct Coord coord, char *str);

struct Coord
str_to_coord(const char *str);

char
rank_to_char(Rank rank);

bool
rank_is_valid(Rank rank);

char
file_to_char(File file);

bool
file_is_valid(File file);

bool
coord_is_valid(const struct Coord coord);

bool
coord_eq(const struct Coord coord1, const struct Coord coord2);

enum Dir
coord_dir(const struct Coord coord1, const struct Coord coord2);

extern const struct Coord COORD_A8;

extern const struct Coord COORD_NONE;

extern const File FILE_NONE;
extern const Rank RANK_NONE;
