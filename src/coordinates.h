#include <stdint.h>
#include <stdbool.h>
#include "direction.h"
#pragma once

typedef uint8_t Rank;
typedef uint8_t File;

struct Coordinates {
	Rank rank;
	File file;
};

bool
coord_exists(const struct Coordinates coord);

bool
coord_eq(const struct Coordinates c1, const struct Coordinates c2);

enum Direction
coord_detect_direction(const struct Coordinates coord);

struct Coordinates
coord_from_algebraic_notation(const char coord[]);

extern const struct Coordinates IMPOSSIBLE;
