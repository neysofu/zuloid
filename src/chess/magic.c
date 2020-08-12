#include "chess/magic.h"
#include "chess/diagonals.h"
#include "chess/bb.h"
#include "utils.h"
#include "chess/generated/magics_bishop.h"
#include "chess/generated/magics_rook.h"
#include "libpopcnt/libpopcnt.h"
#include <inttypes.h>
#include <stdbool.h>
#include "mt-64/mt-64.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HANDLE_ERR(err)                                                                    \
	do {                                                                                   \
		if (err < 0) {                                                                     \
			return -1;                                                                     \
		}                                                                                  \
	} while (0)

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

int64_t BB_SHIFTS_ROOK[64] = { 52, 52 };
int64_t BB_SHIFTS_BISHOP[64] = { 0 };

Bitboard
bb_bishop_magic(Square sq, Bitboard occupancy)
{
	Bitboard mask = BB_MASK_BISHOP[sq] & occupancy;
	size_t i = (mask * BB_MULTIPLIERS_BISHOP[sq]) >> BB_SHIFTS_BISHOP[sq];
	return BB_ATTACKS_BISHOP[i + BB_OFFSETS_BISHOP[sq]];
}

Bitboard
bb_rook_magic(Square sq, Bitboard occupancy)
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

void
bb_init_bishop(const struct Magic *magics)
{
	size_t offset = 0;
	for (Square sq = 0; sq <= SQUARE_MAX; sq++) {
		init_attack_table(sq, magics + sq, BB_ATTACKS_BISHOP + offset, bb_bishop);
		BB_OFFSETS_BISHOP[sq] = offset;
		BB_MASK_BISHOP[sq] = magics[sq].premask;
		BB_SHIFTS_BISHOP[sq] = magics[sq].rshift;
		BB_MULTIPLIERS_BISHOP[sq] = magics[sq].multiplier;
		BB_POSTMASK_BISHOP[sq] = magics[sq].postmask;
		offset += magics[sq].end - magics[sq].start + 3;
	}
}

void
bb_init_rook(const struct Magic *magics)
{
	size_t offset = 0;
	for (Square sq = 0; sq <= SQUARE_MAX; sq++) {
		init_attack_table(sq, magics + sq, BB_ATTACKS_ROOK + offset, bb_rook);
		BB_OFFSETS_ROOK[sq] = offset;
		BB_MASK_ROOK[sq] = magics[sq].premask;
		BB_SHIFTS_ROOK[sq] = magics[sq].rshift;
		BB_MULTIPLIERS_ROOK[sq] = magics[sq].multiplier;
		BB_POSTMASK_ROOK[sq] = magics[sq].postmask;
		offset += magics[sq].end - magics[sq].start + 3;
	}
}

int
magics_export(const struct Magic *magics, const char *identifier, FILE *stream)
{
	int err;
	err = fprintf(stream,
	              "#include \"chess/coordinates.h\"\n"
	              "\n"
	              "const struct Magic %s[SQUARES_COUNT] = {\n",
	              identifier);
	HANDLE_ERR(err);
	for (Square sq = 0; sq < SQUARES_COUNT; sq++) {
		struct Magic magic = magics[sq];
		err = fprintf(stream,
		              "\t[0%o] = { .premask = 0x%" PRIx64 "ULL, .multiplier = 0x%" PRIx64
		              "ULL, .rshift = %d, "
		              ".postmask = "
		              "0x%" PRIx64 "ULL, .start = %zu, .end = %zu },\n",
		              sq,
		              magic.premask,
		              magic.multiplier,
		              magic.rshift,
		              magic.postmask,
		              magic.start,
		              magic.end);
		HANDLE_ERR(err);
	}
	fprintf(stream, "};\n");
	HANDLE_ERR(err);
	return 0;
}

void
magic_init(void)
{
	static bool done = false;
	if (done) {
		return;
	}
	done = true;
	bb_init_rook(MAGICS_ROOK);
	bb_init_bishop(MAGICS_BISHOP);
}

Bitboard
bb_sparse_random(void)
{
	return genrand64_int64() & genrand64_int64() & genrand64_int64();
}

Bitboard
bb_premask_rook(Square sq)
{
	Bitboard x = rank_to_bb(square_rank(sq)) & ~file_to_bb(0) & ~file_to_bb(7);
	Bitboard y = file_to_bb(square_file(sq)) & ~rank_to_bb(0) & ~rank_to_bb(7);
	return (x ^ y) & ~square_to_bb(sq);
}

Bitboard
bb_premask_bishop(Square sq)
{
	Bitboard diagonal1 = square_a1h8_diagonal(sq);
	Bitboard diagonal2 = square_a8h1_diagonal(sq);
	Bitboard borders = file_to_bb(0) | file_to_bb(7) | rank_to_bb(0) | rank_to_bb(7);
	return (diagonal1 ^ diagonal2) & ~borders;
}

size_t
find_start_of_attacks_table(Bitboard attacks_table[])
{
	for (size_t i = 0; i < 4096; i++) {
		if (attacks_table[i]) {
			return i;
		}
	}
	return 4095;
}

size_t
find_end_of_attacks_table(Bitboard attacks_table[])
{
	for (size_t i = 4095; i > 0; i--) {
		if (attacks_table[i]) {
			return i;
		}
	}
	return 0;
}

bool
verify_magic_candidate(const struct Magic *magic,
                       Square sq,
                       Bitboard buffer[],
                       Bitboard (*attacker)(Square, Bitboard))
{
	Bitboard subset = 0;
	do {
		size_t i = (subset * magic->multiplier) >> magic->rshift;
		Bitboard *val = buffer + i;
		Bitboard attacks = attacker(sq, subset);
		if (*val) {
			return false;
		} else {
			*val = attacks;
		}
	} while ((subset = bb_next_subset(magic->premask, subset)));
	return true;
}

void
magic_find_rook(struct Magic *magic, Square square)
{
	size_t attacks_table_size = sizeof(Bitboard) * 4096;
	Bitboard *attacks_table = malloc(sizeof(Bitboard) * 4096);
	exit_if_null(attacks_table);
	magic->premask = bb_premask_rook(square);
	magic->rshift = 52;
	do {
		memset(attacks_table, 0, attacks_table_size);
		magic->multiplier = bb_sparse_random();
	} while (!verify_magic_candidate(magic, square, attacks_table, bb_rook));
	magic->start = find_start_of_attacks_table(attacks_table);
	magic->end = find_end_of_attacks_table(attacks_table);
	free(attacks_table);
}

void
magic_find_bishop(struct Magic *magic, Square square)
{
	size_t attacks_table_size = sizeof(Bitboard) * 4096;
	Bitboard *attacks_table = malloc(sizeof(Bitboard) * 4096);
	exit_if_null(attacks_table);
	magic->premask = bb_premask_bishop(square);
	magic->rshift = 64 - popcount64(magic->premask);
	do {
		memset(attacks_table, 0, attacks_table_size);
		magic->multiplier = bb_sparse_random();
	} while (!verify_magic_candidate(magic, square, attacks_table, bb_bishop));
	magic->start = 0;
	magic->end = find_end_of_attacks_table(attacks_table);
	free(attacks_table);
}
