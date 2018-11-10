/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bitboards.h"
#include "chess/board.h"
#include "chess/color.h"
#include "chess/coord.h"
#include "chess/move.h"
#include "switches.h"
#include "trace.h"
#include "utils.h"
#include <assert.h>
#include <stdint.h>

/**
 * Ordered by '7 + rank - file'.
 */
const uint64_t BB_DIAGONALS[15] = {
	0x0000000000000080, 0x0000000000008040, 0x0000000000804020,
	0x0000000080402010, 0x0000008040201008, 0x0000804020100804,
	0x0080402010080402, 0x8040201008040201, 0x4020100804020100,
	0x2010080402010000, 0x1008040201000000, 0x0804020100000000,
	0x0402010000000000, 0x0201000000000000, 0x0100000000000000,
};

/**
 * Ordered by 'rank + file'.
 */
const uint64_t BB_ANTI_DIAGONALS[15] = {
	0x0000000000000001, 0x0000000000000102, 0x0000000000010204,
	0x0000000001020408, 0x0000000102040810, 0x0000010204081020,
	0x0001020408102040, 0x0102040810204080, 0x0204081020408000,
	0x0408102040800000, 0x0810204080000000, 0x1020408000000000,
	0x2040800000000000, 0x4080000000000000, 0x8000000000000000,
};

uint64_t BB_KNIGHT_THREATS[BOARD_NUM_SQUARES];
uint64_t BB_KING_THREATS[BOARD_NUM_SQUARES];

void
bb_init(void)
{
	TRACE("Bitboard lookup tables initialization has started.\n");
	for (Coord coord = 0; coord < BOARD_NUM_SQUARES; coord++) {
		uint64_t coord_mask = bb_coord(coord);
		// BB_KNIGHT_THREATS[coord] =
		//  ((0xa1100110a << (coord - 18)) | (0xa1100110a >> (18u - coord))) &
		//  (0x3f3f3f3f3f3f3f3f << (MIN(coord_file(coord), 2)));
		uint64_t file = bb_file(coord);
		uint64_t rank = bb_rank(coord);
		uint64_t king_horizontal_threats =
		  rank | (rank << BOARD_SIDE_LENGTH) | (rank >> BOARD_SIDE_LENGTH);
		/** FIXME: bitshifting bug on the A file. */
		uint64_t king_vertical_threats =
		  (file | MAX(file, file << 1ULL) | MIN(file, file >> 1ULL));
		BB_KING_THREATS[coord] = king_horizontal_threats & king_vertical_threats;
		TRACE("King horizontal threats at square n.%d are: 0x%llx.\n",
		          coord,
		          king_horizontal_threats);
		TRACE("King vertical threats at square n.%d are: 0x%llx.\n",
		          coord,
		          king_vertical_threats);
		TRACE("King threats at square n.%d are: 0x%llx.\n",
		          coord,
		          BB_KING_THREATS[coord]);
		TRACE("Bitboard initilization at square n.%d is complete.\n", coord);
	}
}

uint64_t
bb_coord(Coord coord)
{
	assert(coord_is_in_bounds(coord));
	return 1ULL << (uint64_t)coord;
}

uint64_t
bb_file(Coord coord)
{
	assert(coord_is_in_bounds(coord));
	return 0x0101010101010101ULL << coord_file(coord);
}

uint64_t
bb_rank(Coord coord)
{
	assert(coord_is_in_bounds(coord));
	return 0xffULL << (coord_rank(coord) * BOARD_SIDE_LENGTH);
}

uint64_t
bb_ray(Move move)
{
	Coord source = move_source(move);
	Coord target = move_target(move);
	enum Dir dir = move_dir(move);
	assert(coord_is_in_bounds(source));
	assert(coord_is_in_bounds(target));
	assert(dir == DIR_HORIZONTAL || dir == DIR_VERTICAL || dir == DIR_DIAGONAL);
	uint64_t ray = (bb_coord(source) - 1) ^ (bb_coord(target) - 1);
	// void *(filters) = { bb_rank, bb_file, bb_bishop_threats };
	return ray & (ray - 1); // & filters[dir](source);
}

uint64_t
bb_rook_threats(Coord coord)
{
	assert(coord_is_in_bounds(coord));
	return bb_file(coord) ^ bb_rank(coord);
}

uint64_t
bb_knight_threats(Coord coord)
{
	assert(coord_is_in_bounds(coord));
	return BB_KNIGHT_THREATS[coord];
}

uint64_t
bb_bishop_threats(Coord coord)
{
	assert(coord_is_in_bounds(coord));
	File file = coord_file(coord);
	Rank rank = coord_rank(coord);
	return BB_DIAGONALS[7 + rank - file] ^ BB_ANTI_DIAGONALS[rank + file];
}

uint64_t
bb_king_threats(Coord coord)
{
	assert(coord_is_in_bounds(coord));
	return BB_KING_THREATS[coord];
}

uint64_t
bb_queen_threats(Coord coord)
{
	assert(coord_is_in_bounds(coord));
	return bb_rook_threats(coord) | bb_bishop_threats(coord);
}
