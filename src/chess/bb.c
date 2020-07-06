#include "chess/bb.h"
#include "mt-64/mt-64.h"
#include "utils.h"
#include <assert.h>
#include <libpopcnt/libpopcnt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

Bitboard BB_KNIGHT_ATTACKS[0100] = { 0 };
Bitboard BB_KING_ATTACKS[0100] = { 0 };

void
bb_print(Bitboard value)
{
	for (Rank rank = RANK_MAX; rank >= 0; rank--) {
		for (File file = 0; file <= FILE_MAX; file++) {
			putchar(value & square_to_bb(square_new(file, rank)) ? 'X' : '.');
			putchar(' ');
		}
		puts("");
	}
	puts("");
}

Bitboard BB_BISHOP_MASK[0100] = { 0 };
Bitboard BB_ROOK_MASK[0100] = { 0 };

int64_t BB_BISHOP_MULTIPLIER[0100] = { 0 };
int64_t BB_ROOK_MULTILIER[0100] = { 0 };

int64_t BB_BISHOP_SHIFT[0100] = { 0 };
int64_t BB_ROOK_SHIFT[0100] = { 0 };

struct Magic
{
	Bitboard magic;
	Bitboard mask;
	Bitboard shift;
	Bitboard *offset;
};

struct Magic BishopTable[SQUARES_COUNT];
struct Magic RookTable[SQUARES_COUNT];

const Bitboard RookMagics[64] = {
	0xA180022080400230ull, 0x0040100040022000ull, 0x0080088020001002ull,
	0x0080080280841000ull, 0x4200042010460008ull, 0x04800A0003040080ull,
	0x0400110082041008ull, 0x008000A041000880ull, 0x10138001A080C010ull,
	0x0000804008200480ull, 0x00010011012000C0ull, 0x0022004128102200ull,
	0x000200081201200Cull, 0x202A001048460004ull, 0x0081000100420004ull,
	0x4000800380004500ull, 0x0000208002904001ull, 0x0090004040026008ull,
	0x0208808010002001ull, 0x2002020020704940ull, 0x8048010008110005ull,
	0x6820808004002200ull, 0x0A80040008023011ull, 0x00B1460000811044ull,
	0x4204400080008EA0ull, 0xB002400180200184ull, 0x2020200080100380ull,
	0x0010080080100080ull, 0x2204080080800400ull, 0x0000A40080360080ull,
	0x02040604002810B1ull, 0x008C218600004104ull, 0x8180004000402000ull,
	0x488C402000401001ull, 0x4018A00080801004ull, 0x1230002105001008ull,
	0x8904800800800400ull, 0x0042000C42003810ull, 0x008408110400B012ull,
	0x0018086182000401ull, 0x2240088020C28000ull, 0x001001201040C004ull,
	0x0A02008010420020ull, 0x0010003009010060ull, 0x0004008008008014ull,
	0x0080020004008080ull, 0x0282020001008080ull, 0x50000181204A0004ull,
	0x48FFFE99FECFAA00ull, 0x48FFFE99FECFAA00ull, 0x497FFFADFF9C2E00ull,
	0x613FFFDDFFCE9200ull, 0xFFFFFFE9FFE7CE00ull, 0xFFFFFFF5FFF3E600ull,
	0x0010301802830400ull, 0x510FFFF5F63C96A0ull, 0xEBFFFFB9FF9FC526ull,
	0x61FFFEDDFEEDAEAEull, 0x53BFFFEDFFDEB1A2ull, 0x127FFFB9FFDFB5F6ull,
	0x411FFFDDFFDBF4D6ull, 0x0801000804000603ull, 0x0003FFEF27EEBE74ull,
	0x7645FFFECBFEA79Eull,
};

const Bitboard BishopMagics[64] = {
	0xFFEDF9FD7CFCFFFFull, 0xFC0962854A77F576ull, 0x5822022042000000ull,
	0x2CA804A100200020ull, 0x0204042200000900ull, 0x2002121024000002ull,
	0xFC0A66C64A7EF576ull, 0x7FFDFDFCBD79FFFFull, 0xFC0846A64A34FFF6ull,
	0xFC087A874A3CF7F6ull, 0x1001080204002100ull, 0x1810080489021800ull,
	0x0062040420010A00ull, 0x5028043004300020ull, 0xFC0864AE59B4FF76ull,
	0x3C0860AF4B35FF76ull, 0x73C01AF56CF4CFFBull, 0x41A01CFAD64AAFFCull,
	0x040C0422080A0598ull, 0x4228020082004050ull, 0x0200800400E00100ull,
	0x020B001230021040ull, 0x7C0C028F5B34FF76ull, 0xFC0A028E5AB4DF76ull,
	0x0020208050A42180ull, 0x001004804B280200ull, 0x2048020024040010ull,
	0x0102C04004010200ull, 0x020408204C002010ull, 0x02411100020080C1ull,
	0x102A008084042100ull, 0x0941030000A09846ull, 0x0244100800400200ull,
	0x4000901010080696ull, 0x0000280404180020ull, 0x0800042008240100ull,
	0x0220008400088020ull, 0x04020182000904C9ull, 0x0023010400020600ull,
	0x0041040020110302ull, 0xDCEFD9B54BFCC09Full, 0xF95FFA765AFD602Bull,
	0x1401210240484800ull, 0x0022244208010080ull, 0x1105040104000210ull,
	0x2040088800C40081ull, 0x43FF9A5CF4CA0C01ull, 0x4BFFCD8E7C587601ull,
	0xFC0FF2865334F576ull, 0xFC0BF6CE5924F576ull, 0x80000B0401040402ull,
	0x0020004821880A00ull, 0x8200002022440100ull, 0x0009431801010068ull,
	0xC3FFB7DC36CA8C89ull, 0xC3FF8A54F4CA2C89ull, 0xFFFFFCFCFD79EDFFull,
	0xFC0863FCCB147576ull, 0x040C000022013020ull, 0x2000104000420600ull,
	0x0400000260142410ull, 0x0800633408100500ull, 0xFC087E8E4BB2F736ull,
	0x43FF9E4EF4CA2C89ull,
};

Bitboard BishopAttacks[0x1480];
Bitboard RookAttacks[0x19000];

Bitboard
slider_index(Bitboard occupied, struct Magic *table)
{
	return ((occupied & table->mask) * table->magic) >> table->shift;
}

Bitboard
slider_attacks(Square sq, Bitboard occupancy, const short delta[4][2])
{
	Bitboard bb = 0;
	for (int i = 0; i < 4; i++) {
		int df = delta[i][0];
		int dr = delta[i][1];
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
bb_bishop(Square sq, Bitboard occupancy)
{
	const short bishop_offsets[4][2] = { { -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 1 } };
	return slider_attacks(sq, occupancy, bishop_offsets);
}

Bitboard
bb_rook(Square sq, Bitboard occupancy)
{
	const short rook_offsets[4][2] = { { -1, 0 }, { 0, -1 }, { 0, 1 }, { 1, 0 } };
	return slider_attacks(sq, occupancy, rook_offsets);
}

void
init_slider_attacks(Square sq,
                    struct Magic *table,
                    Bitboard magic,
                    const short offsets[4][2])
{
	Bitboard edges = ((rank_to_bb(0) | rank_to_bb(7)) & ~rank_to_bb(square_rank(sq))) |
	                 ((file_to_bb(0) | file_to_bb(7)) & ~file_to_bb(square_file(sq)));
	Bitboard occupied = 0;
	/* Init entry for the given square. */
	table[sq].magic = magic;
	table[sq].mask = slider_attacks(sq, 0, offsets) & ~edges;
	table[sq].shift = 64 - popcnt64(table[sq].mask);
	/* Track the offset as we use up the table. */
	if (sq != 63) {
		table[sq + 1].offset = table[sq].offset + (1 << popcount64(table[sq].mask));
	}
	do { /* Init attacks for all occupancy variations. */
		int index = slider_index(occupied, &table[sq]);
		table[sq].offset[index] = slider_attacks(sq, occupied, offsets);
		occupied = (occupied - table[sq].mask) & table[sq].mask;
	} while (occupied);
}

void
bb_init_knight(void)
{
	const short knight_offsets[8][2] = {
		{ -2, -1 }, { -2, 1 }, { 2, -1 }, { 2, 1 },
		{ -1, -2 }, { -1, 2 }, { 1, -2 }, { 1, 2 },
	};
	for (Square square = 0; square <= SQUARE_MAX; square++) {
		Bitboard bb = 0;
		for (int i = 0; i < 8; i++) {
			int f = square_file(square) + knight_offsets[i][0];
			int r = square_rank(square) + knight_offsets[i][1];
			if (r >= 0 && f >= 0 && r < 8 && f < 8) {
				bb |= square_to_bb(square_new(f, r));
			}
		}
		BB_KNIGHT_ATTACKS[square] = bb;
	}
}

void
bb_init_king(void)
{
	const short king_offsets[8][2] = {
		{ -1, -1 }, { 0, -1 }, { 1, -1 }, { -1, 0 },
		{ 1, 0 },   { -1, 1 }, { 0, 1 },  { 1, 1 },
	};
	for (Square square = 0; square <= SQUARE_MAX; square++) {
		Bitboard bb = 0;
		for (int i = 0; i < 8; i++) {
			int f = square_file(square) + king_offsets[i][0];
			int r = square_rank(square) + king_offsets[i][1];
			if (r >= 0 && f >= 0 && r <= RANK_MAX && f <= FILE_MAX) {
				bb |= square_to_bb(square_new(f, r));
			}
		}
		BB_KING_ATTACKS[square] = bb;
	}
}

void
bb_init(void)
{
	const short bishop_offsets[4][2] = { { -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 1 } };
	const short rook_offsets[4][2] = { { -1, 0 }, { 0, -1 }, { 0, 1 }, { 1, 0 } };
	bb_init_knight();
	bb_init_king();
}
