#include <stdint.h>
#include "bitboards.h"
#include "board.h"
#include "color.h"
#include "coord.h"

const uint64_t BB_FILES[BOARD_SIDE_LENGTH] = {
	0x0101010101010101,
	0x0202020202020202,
	0x0404040404040404,
	0x0808080808080808,
	0x1010101010101010,
	0x2020202020202020,
	0x4040404040404040,
	0x8080808080808080,
};

uint64_t BB_ADJIACTENT_FILES[BOARD_SIDE_LENGTH];

const uint64_t BB_RANKS[BOARD_SIDE_LENGTH] = {
	0x00000000000000ff,
	0x000000000000ff00,
	0x0000000000ff0000,
	0x00000000ff000000,
	0x000000ff00000000,
	0x0000ff0000000000,
	0x00ff000000000000,
	0xff00000000000000,
};

uint64_t BB_ADJIACTENT_RANKS[BOARD_SIDE_LENGTH];

// Ordered by '7 + rank - file'.
const uint64_t BB_DIAGONALS_MAIN[15] = {
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

// Ordered by 'rank - file'.
const uint64_t BB_DIAGONALS_ANTI[15] = {
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

uint64_t BB_ROOK_ATTACKS[BOARD_NUM_SQUARES];
uint64_t BB_KNIGHT_ATTACKS[BOARD_NUM_SQUARES];
uint64_t BB_BISHOP_ATTACKS[BOARD_NUM_SQUARES];
uint64_t BB_KING_ATTACKS[BOARD_NUM_SQUARES];
uint64_t BB_PAWN_ATTACKS[BOARD_NUM_SQUARES];
uint64_t BB_PAWN_MOVES[BOARD_NUM_SQUARES];

void
bb_init(void) {
	BB_ADJIACTENT_FILES[0] = BB_FILES[1];
	BB_ADJIACTENT_FILES[1] = BB_FILES[0] | BB_FILES[2];
	BB_ADJIACTENT_FILES[2] = BB_FILES[1] | BB_FILES[3];
	BB_ADJIACTENT_FILES[3] = BB_FILES[2] | BB_FILES[4];
	BB_ADJIACTENT_FILES[4] = BB_FILES[3] | BB_FILES[5];
	BB_ADJIACTENT_FILES[5] = BB_FILES[4] | BB_FILES[6];
	BB_ADJIACTENT_FILES[6] = BB_FILES[5] | BB_FILES[7];
	BB_ADJIACTENT_FILES[7] = BB_FILES[6];
	BB_ADJIACTENT_RANKS[0] = BB_RANKS[1];
	BB_ADJIACTENT_RANKS[0] = BB_RANKS[0] | BB_RANKS[2];
	BB_ADJIACTENT_RANKS[0] = BB_RANKS[1] | BB_RANKS[3];
	BB_ADJIACTENT_RANKS[0] = BB_RANKS[2] | BB_RANKS[4];
	BB_ADJIACTENT_RANKS[0] = BB_RANKS[3] | BB_RANKS[5];
	BB_ADJIACTENT_RANKS[0] = BB_RANKS[4] | BB_RANKS[6];
	BB_ADJIACTENT_RANKS[0] = BB_RANKS[5] | BB_RANKS[7];
	BB_ADJIACTENT_RANKS[0] = BB_RANKS[6];
	Coord coord;
	for (coord = 0; coord < BOARD_NUM_SQUARES; coord++) {
		BB_ROOK_ATTACKS[coord] = BB_FILES[coord_file(coord)] ^
			                     BB_RANKS[coord_rank(coord)];
		BB_KNIGHT_ATTACKS[coord] = ((coord <<  6) &
				                    BB_ADJIACTENT_RANKS[coord_file(coord)]) |
		              			   ((coord >>  6) &
									BB_ADJIACTENT_RANKS[coord_file(coord)]) |
		              			   ((coord << 15) &
									BB_ADJIACTENT_FILES[coord_file(coord)]) |
		              			   ((coord >> 15) &
									BB_ADJIACTENT_FILES[coord_file(coord)]) |
		              			   ((coord << 17) &
									BB_ADJIACTENT_FILES[coord_file(coord)]) |
		              			   ((coord >> 17) &
									BB_ADJIACTENT_FILES[coord_file(coord)]) |
		              			   ((coord << 10) &
									BB_ADJIACTENT_RANKS[coord_file(coord)]) |
		              			   ((coord >> 10) &
									BB_ADJIACTENT_RANKS[coord_file(coord)]);
		BB_ROOK_ATTACKS[coord] = bb_diagonal_main(coord) ^
			                     bb_diagonal_anti(coord);
		BB_KING_ATTACKS[coord] = ((BB_FILES[coord_file(coord)] |
				                   BB_ADJIACTENT_FILES[coord_file(coord)]) &
		                          (BB_RANKS[coord_rank(coord)] |
				                   BB_ADJIACTENT_RANKS[coord_rank(coord)])) ^
								 coord;
	}
}

uint64_t
bb_file(Coord coord) {
	return BB_FILES[coord_file(coord)];
}

uint64_t
bb_rank(Coord coord) {
	return BB_RANKS[coord_rank(coord)];
}

uint64_t
bb_diagonal_main(Coord coord) {
	return BB_DIAGONALS_MAIN[7 + coord_rank(coord) - coord_file(coord)];
}

uint64_t
bb_diagonal_anti(Coord coord) {
	return BB_DIAGONALS_ANTI[coord_file(coord) + coord_rank(coord)];
}

uint64_t
bb_rook_attacks(Coord coord) {
	return BB_ROOK_ATTACKS[coord];
}

uint64_t
bb_knight_attacks(Coord coord) {
	return BB_KNIGHT_ATTACKS[coord];
}

uint64_t
bb_bishop_attacks(Coord coord) {
	return BB_BISHOP_ATTACKS[coord];
}

uint64_t
bb_king_attacks(Coord coord) {
	return BB_KING_ATTACKS[coord];
}

uint64_t
bb_queen_attacks(Coord coord) {
	return bb_rook_attacks(coord) | bb_bishop_attacks(coord);
}

uint64_t
bb_pawn_moves(Coord coord, enum Color color) {
	return color ? coord - BOARD_NUM_SQUARES : coord + BOARD_NUM_SQUARES;
}
