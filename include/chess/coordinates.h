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
typedef int_fast8_t File;
typedef int_fast8_t Rank;
typedef int_fast8_t Square;
typedef uint_fast64_t Bitboard;

static const File FILE_MAX = 7;
static const Rank RANK_MAX = 7;
static const Square SQUARE_MAX = 63;

static const int FILES_COUNT = 8;
static const int RANKS_COUNT = 8;
static const int SQUARES_COUNT = 64;

static const File FILE_NONE = -1;
static const Rank RANK_NONE = -1;
static const Square SQUARE_NONE = -1;

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
square_to_bitboard(Square sq);
Bitboard
file_to_bitboard(File file);
Bitboard
rank_to_bitboard(Rank rank);

#endif
