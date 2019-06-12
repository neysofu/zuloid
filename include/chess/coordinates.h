/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Useful resources:
 *  -  https://www.chessprogramming.org/Bitboards
 *  -  https://www.chessprogramming.org/General_Setwise_Operations
 *  -  https://en.wikipedia.org/wiki/Bitboard
 *  -  https://graphics.stanford.edu/~seander/bithacks.html
 *  -  https://cinnamonchess.altervista.org/bitboard_calculator/Calc.html */

#ifndef Z64C_CHESS_COORDINATES_H
#define Z64C_CHESS_COORDINATES_H

#include <stdint.h>

/* Squares are enumerated in a little-endian and file-first fashion, with the following
 * compass rose:
 *
 *    -7  +1  +9
 *      \  |  /
 *   -8 <- 0 -> +8
 *      /  |  \
 *    -9  -1  +7
 *
 */
typedef int8_t File;
typedef int8_t Rank;
typedef int8_t Square;
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
File
square_file(Square sq);
Rank
square_rank(Square sq);

Bitboard
square_to_bb(Square sq);
Bitboard
file_to_bb(File file);
Bitboard
rank_to_bb(Rank rank);

Bitboard
square_knight_threats(Square sq);

static const Bitboard BB_LIGHT_SQUARES = 0xaaaaaaaaaaaaaaaa;
static const Bitboard BB_DARK_SQUARES = 0x5555555555555555;
static const Bitboard BB_CENTER_SQUARES = 0x183c3c180000;

#endif
