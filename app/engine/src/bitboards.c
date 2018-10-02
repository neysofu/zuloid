#include <assert.h>
#include <stdint.h>
#include "bitboards.h"
#include "chess/board.h"
#include "chess/color.h"
#include "chess/coord.h"
#include "chess/move.h"
#include "utils.h"

uint64_t BB_ADJIACTENT_FILES[BOARD_SIDE_LENGTH] = {
	0x0202020202020202,
	0x0505050505050505,
	0x0a0a0a0a0a0a0a0a,
	0x1414141414141414,
	0x2828282828282828,
	0x5050505050505050,
	0xa0a0a0a0a0a0a0a0,
	0x4040404040404040,
};

uint64_t BB_ADJIACTENT_RANKS[BOARD_SIDE_LENGTH] = {
	0x000000000000ff00,
	0x0000000000ff00ff,
	0x00000000ff00ff00,
	0x000000ff00ff0000,
	0x0000ff00ff000000,
	0x00ff00ff00000000,
	0xff00ff0000000000,
	0x00ff000000000000,
};

// Ordered by '7 + rank - file'.
const uint64_t BB_DIAGONALS[15] = {
	0x0000000000000080,
	0x0000000000008040,
	0x0000000000804020,
	0x0000000080402010,
	0x0000008040201008,
	0x0000804020100804,
	0x0080402010080402,
	0x8040201008040201,
	0x4020100804020100,
	0x2010080402010000,
	0x1008040201000000,
	0x0804020100000000,
	0x0402010000000000,
	0x0201000000000000,
	0x0100000000000000,
};

// Ordered by 'rank + file'.
const uint64_t BB_ANTI_DIAGONALS[15] = {
	0x0000000000000001,
	0x0000000000000102,
	0x0000000000010204,
	0x0000000001020408,
	0x0000000102040810,
	0x0000010204081020,
	0x0001020408102040,
	0x0102040810204080,
	0x0204081020408000,
	0x0408102040800000,
	0x0810204080000000,
	0x1020408000000000,
	0x2040800000000000,
	0x4080000000000000,
	0x8000000000000000,
};

uint64_t BB_KNIGHT_THREATS[BOARD_NUM_SQUARES];
uint64_t BB_KING_THREATS[BOARD_NUM_SQUARES];

void
bb_init(void) {
	static bool flag = false;
	if (flag) {
		return;
	} else {
		flag = true;
	}
	for (Coord coord = 0; coord < BOARD_NUM_SQUARES; coord++) {
		uint64_t coord_mask = bb_coord(coord);
		BB_KNIGHT_THREATS[coord] = ((bb_coord(coord) << 6) &
				                    BB_ADJIACTENT_RANKS[coord_file(coord)]) |
		              			   ((bb_coord(coord) >>  6) &
									BB_ADJIACTENT_RANKS[coord_file(coord)]) |
		              			   ((bb_coord(coord) << 15) &
									BB_ADJIACTENT_FILES[coord_file(coord)]) |
		              			   ((bb_coord(coord) >> 15) &
									BB_ADJIACTENT_FILES[coord_file(coord)]) |
		              			   ((bb_coord(coord) << 17) &
									BB_ADJIACTENT_FILES[coord_file(coord)]) |
		              			   ((bb_coord(coord) >> 17) &
									BB_ADJIACTENT_FILES[coord_file(coord)]) |
		              			   ((bb_coord(coord) << 10) &
									BB_ADJIACTENT_RANKS[coord_file(coord)]) |
		              			   ((bb_coord(coord) >> 10) &
									BB_ADJIACTENT_RANKS[coord_file(coord)]);
		BB_KING_THREATS[coord] = coord ^
								 ((bb_file(coord) |
				                   BB_ADJIACTENT_FILES[coord_file(coord)]) &
		                          (bb_rank(coord) |
				                   BB_ADJIACTENT_RANKS[coord_rank(coord)]));
	}
}

uint64_t
bb_coord(Coord coord) {
	assert(coord_is_in_bounds(coord));
	return 1ULL << (uint64_t)(coord);
}

uint64_t
bb_file(Coord coord) {
	assert(coord_is_in_bounds(coord));
	return 0x0101010101010101 << coord_file(coord);
}

uint64_t
bb_rank(Coord coord) {
	assert(coord_is_in_bounds(coord));
	return 0xff << (coord_rank(coord) << BOARD_SIDE_LENGTH);
}

uint64_t
bb_diagonals(Coord coord) {
	assert(coord_is_in_bounds(coord));
	return BB_DIAGONALS[7 + coord_rank(coord) - coord_file(coord)] |
		   BB_ANTI_DIAGONALS[coord_rank(coord) + coord_file(coord)];
}

uint64_t
bb_ray(Move move) {
	Coord source = move_source(move);
	Coord target = move_target(move);
	enum Dir dir = move_dir(move);
	assert(coord_is_in_bounds(source));
	assert(coord_is_in_bounds(target));
	assert(dir == DIR_HORIZONTAL ||
		   dir == DIR_VERTICAL ||
		   dir == DIR_DIAGONAL);
	uint64_t ray = (bb_coord(source) - 1) ^ (bb_coord(target) - 1);
	switch (dir) {
		case DIR_HORIZONTAL:
			ray &= bb_rank(source);
			break;
		case DIR_VERTICAL:
			ray &= bb_file(source);
			break;
		case DIR_DIAGONAL:
			ray &= bb_diagonals(source);
			break;
		default:
			assert(false);
	}
	// Clear lowest set bit.
	ray &= ray - 1;
	return ray;
}

uint64_t
bb_rook_threats(Coord coord) {
	assert(coord_is_in_bounds(coord));
	return bb_file(coord) ^ bb_rank(coord);
}

uint64_t
bb_knight_threats(Coord coord) {
	assert(coord_is_in_bounds(coord));
	return BB_KNIGHT_THREATS[coord];
}

uint64_t
bb_bishop_threats(Coord coord) {
	assert(coord_is_in_bounds(coord));
	return bb_diagonals(coord);
}

uint64_t
bb_king_threats(Coord coord) {
	assert(coord_is_in_bounds(coord));
	return BB_KING_THREATS[coord];
}

uint64_t
bb_queen_threats(Coord coord) {
	assert(coord_is_in_bounds(coord));
	return bb_rook_threats(coord) | bb_bishop_threats(coord);
}
