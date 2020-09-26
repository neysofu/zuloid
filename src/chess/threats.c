/* SPDX-License-Identifier: GPL-3.0-only */

#include "chess/bb.h"
#include "chess/coordinates.h"
#include "chess/generated/magics_bishop.h"
#include "chess/generated/magics_rook.h"
#include "chess/magic.h"
#include <stdbool.h>
#include <stdlib.h>

Bitboard THREATS_BY_KNIGHT[SQUARES_COUNT] = { 0 };
Bitboard THREATS_BY_KING[SQUARES_COUNT] = { 0 };

Bitboard BB_ATTACKS_BISHOP[64 * 4096] = { 0 };
Bitboard BB_ATTACKS_ROOK[64 * 4096] = { 0 };

Bitboard BB_MASK_BISHOP[SQUARES_COUNT] = { 0 };
Bitboard BB_MASK_ROOK[SQUARES_COUNT] = { 0 };
Bitboard BB_POSTMASK_ROOK[SQUARES_COUNT] = { 0 };
Bitboard BB_POSTMASK_BISHOP[SQUARES_COUNT] = { 0 };

int64_t BB_MULTIPLIERS_BISHOP[64] = { 0 };
int64_t BB_MULTIPLIERS_ROOK[64] = { 0 };

size_t BB_OFFSETS_ROOK[64] = { 0 };
size_t BB_OFFSETS_BISHOP[64] = { 0 };

int64_t BB_SHIFTS_ROOK[64] = { 0 };
int64_t BB_SHIFTS_BISHOP[64] = { 0 };

Bitboard
bb_attacks_by_offsets(Square sq, const short offsets[8][2])
{
	Bitboard bb = 0;
	for (size_t i = 0; i < 8; i++) {
		File f = square_file(sq) + offsets[i][0];
		Rank r = square_rank(sq) + offsets[i][1];
		if (r >= 0 && f >= 0 && r <= RANK_MAX && f <= FILE_MAX) {
			bb |= square_to_bb(square_new(f, r));
		}
	}
	return bb;
}

Bitboard
slider_attacks(Square sq, Bitboard occupancy, const short delta[4][2])
{
	Bitboard bb = 0;
	for (int i = 0; i < 4; i++) {
		const int df = delta[i][0];
		const int dr = delta[i][1];
		File file = square_file(sq) + df;
		Rank rank = square_rank(sq) + dr;
		while (file >= 0 && file < 8 && rank >= 0 && rank < 8) {
			bb |= square_to_bb(square_new(file, rank));
			if (square_to_bb(square_new(file, rank)) & occupancy) {
				break;
			}
			rank += dr;
			file += df;
		}
	}
	return bb;
}

Bitboard
threats_by_king(Square sq)
{
	return THREATS_BY_KING[sq];
}

Bitboard
threats_by_knight(Square sq)
{
	return THREATS_BY_KNIGHT[sq];
}

Bitboard
threats_by_rook_no_init(Square sq, Bitboard occupancy)
{
	const short OFFSETS_ROOK[4][2] = { { 1, 0 }, { 0, 1 }, { 0, -1 }, { -1, 0 } };
	return slider_attacks(sq, occupancy, OFFSETS_ROOK);
}

Bitboard
threats_by_bishop_no_init(Square sq, Bitboard occupancy)
{
	const short OFFSETS_BISHOP[4][2] = { { 1, -1 }, { -1, 1 }, { 1, 1 }, { -1, -1 } };
	return slider_attacks(sq, occupancy, OFFSETS_BISHOP);
}

Bitboard
threats_by_bishop(Square sq, Bitboard occupancy)
{
	Bitboard mask = BB_MASK_BISHOP[sq] & occupancy;
	size_t i = (mask * BB_MULTIPLIERS_BISHOP[sq]) >> BB_SHIFTS_BISHOP[sq];
	return BB_ATTACKS_BISHOP[i + BB_OFFSETS_BISHOP[sq]];
}

Bitboard
threats_by_rook(Square sq, Bitboard occupancy)
{
	Bitboard mask = BB_MASK_ROOK[sq] & occupancy;
	size_t i = (mask * BB_MULTIPLIERS_ROOK[sq]) >> BB_SHIFTS_ROOK[sq];
	return BB_ATTACKS_ROOK[i + BB_OFFSETS_ROOK[sq]];
}

void
init_attack_table(Square sq,
                  const struct Magic *magic,
                  Bitboard attacks_table[],
                  Bitboard (*slider)(Square, Bitboard))
{

	Bitboard subset = 0;
	do {
		size_t i = (subset * magic->multiplier) >> magic->rshift;
		attacks_table[i] = slider(sq, subset);
	} while ((subset = bb_next_subset(magic->premask, subset)));
}

size_t
magic_calculate_offset(const struct Magic *magic)
{
	return 1 << (64 - magic->rshift);
}

void
bb_init_bishop(const struct Magic *magics)
{
	size_t offset = 0;
	for (Square sq = 0; sq <= SQUARE_MAX; sq++) {
		init_attack_table(
		  sq, magics + sq, BB_ATTACKS_BISHOP + offset, threats_by_bishop_no_init);
		BB_OFFSETS_BISHOP[sq] = offset;
		BB_MASK_BISHOP[sq] = magics[sq].premask;
		BB_SHIFTS_BISHOP[sq] = magics[sq].rshift;
		BB_MULTIPLIERS_BISHOP[sq] = magics[sq].multiplier;
		BB_POSTMASK_BISHOP[sq] = magics[sq].postmask;
		offset += magic_calculate_offset(magics + sq);
	}
}

void
bb_init_rook(const struct Magic *magics)
{
	size_t offset = 0;
	for (Square sq = 0; sq <= SQUARE_MAX; sq++) {
		init_attack_table(
		  sq, magics + sq, BB_ATTACKS_ROOK + offset, threats_by_rook_no_init);
		BB_OFFSETS_ROOK[sq] = offset;
		BB_MASK_ROOK[sq] = magics[sq].premask;
		BB_SHIFTS_ROOK[sq] = magics[sq].rshift;
		BB_MULTIPLIERS_ROOK[sq] = magics[sq].multiplier;
		BB_POSTMASK_ROOK[sq] = magics[sq].postmask;
		offset += magic_calculate_offset(magics + sq);
	}
}

void
init_threats(void)
{
	static bool done = false;
	if (done) {
		return;
	}
	done = true;
	// Keep in mind that both knights and kings can move in 8 different directions.
	const short OFFSETS_KNIGHT[8][2] = {
		{ -2, -1 }, { -2, 1 }, { 2, -1 }, { 2, 1 },
		{ -1, -2 }, { -1, 2 }, { 1, -2 }, { 1, 2 },
	};

	const short OFFSETS_KING[8][2] = {
		{ -1, -1 }, { 0, -1 }, { 1, -1 }, { -1, 0 },
		{ 1, 0 },   { -1, 1 }, { 0, 1 },  { 1, 1 },
	};
	for (Square sq = 0; sq <= SQUARE_MAX; sq++) {
		THREATS_BY_KNIGHT[sq] = bb_attacks_by_offsets(sq, OFFSETS_KNIGHT);
		THREATS_BY_KING[sq] = bb_attacks_by_offsets(sq, OFFSETS_KING);
	}
	bb_init_rook(MAGICS_ROOK);
	bb_init_bishop(MAGICS_BISHOP);
}
