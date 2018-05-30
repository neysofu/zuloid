#include <stdbool.h>
#include "coordinates.h"
#include "chessboard.h"

bool
coord_eq(const struct Coordinates c1, const struct Coordinates c2) {
	return c1.rank == c2.rank && c1.file == c2.file;
}

bool
coord_exists(const struct Coordinates coord) {
	return coord.rank < CHESSBOARD_SIDE_LENGHT && coord.file < CHESSBOARD_SIDE_LENGHT;
}

struct Coordinates
coord_from_algebraic_notation(const char coord[]) {
	return (struct Coordinates) {
		.file = coord[0] - 'a',
		.rank = coord[1] - '1',
	};
}

const struct Coordinates IMPOSSIBLE = {
	.rank = 42,
	.file = 42,
};
