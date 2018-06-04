#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "dir.h"

typedef uint8_t Rank;
typedef uint8_t File;

struct Coord {
	Rank rank;
	File file;
};

struct Coord
coord_new(Rank rank, File file);

char *
coord_to_str(const struct Coord coord);

struct Coord
str_to_coord(const char *coord);

char
rank_to_char(Rank rank);

char
file_to_char(File file);

bool
coord_is_valid(const struct Coord coord);

bool
coord_eq(const struct Coord coord1, const struct Coord coord2);

enum Direction
coord_dir(const struct Coord coord1, const struct Coord coord2);

extern const struct Coord COORD_NONE;
