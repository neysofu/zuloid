#ifndef ZULOID_CHESS_COORDINATES_H
#define ZULOID_CHESS_COORDINATES_H

#include <stdint.h>

typedef int8_t File;
typedef int8_t Rank;

// Squares are enumerated in a little-endian and file-first fashion (also known
// as LEFR), with the following compass rose:
//
//   +---------------+
//   |  -7  +1  +9   |
//   |    \  |  /    |
//   | -8 <- 0 -> +8 |
//   |    /  |  \    |
//   |  -9  -1  +7   |
//   +---------------+
typedef int_fast8_t Square;
typedef uint64_t Bitboard;

enum
{
	FILE_MAX = 7,
	RANK_MAX = 7,
	SQUARE_MAX = 63,

	FILE_NONE = -1,
	RANK_NONE = -1,
	SQUARE_NONE = -1,

	FILES_COUNT = 8,
	RANKS_COUNT = 8,
	SQUARES_COUNT = 64,
};

char
file_to_char(File f);
char
rank_to_char(Rank r);
File
char_to_file(char c);
Rank
char_to_rank(char c);

Square
square_new(File f, Rank r);
Square
square_from_str(const char *str);
File
square_file(Square sq);
Rank
square_rank(Square sq);

Bitboard
square_to_bb(Square sq);
Square
bb_to_square(Bitboard bb);
Bitboard
file_to_bb(File file);
Bitboard
rank_to_bb(Rank rank);

Square
lsb_to_square(int lsb);

Square
msb_to_square(int msb);

static const Bitboard BB_LIGHT_SQUARES = 0xaaaaaaaaaaaaaaaa;
static const Bitboard BB_DARK_SQUARES = 0x5555555555555555;
static const Bitboard BB_CENTER_SQUARES = 0x183c3c180000;

#endif
