#pragma once

#include "coordinates.h"
#include "direction.h"
#include "piece.h"

struct Move {
	struct Coordinates from;
	struct Coordinates to;
	enum Direction direction;
	enum Piece promotion;
};

struct Move
mv_init(const struct Coordinates from, const struct Coordinates to);

bool
mv_is_idempotent(const struct Move mv);

enum Direction
mv_set_direction(struct Move mv);

uint8_t
mv_rank_change(const struct Move mv);

uint8_t
mv_file_change(const struct Move mv);
