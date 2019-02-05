/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chess/coordinates.h"
#include "chess/color.h"

#define FILE_TO_BITBOARD(file) (0xffULL << ((file) << 3))
#define RANK_TO_BITBOARD(rank) (0x0101010101010101 << (rank))
#define SQUARE_TO_BITBOARD(square) (0x1ULL << (square))

char
rank_to_char(Rank r)
{
	return r + '1';
}

char
file_to_char(File f)
{
	return f + 'a';
}

File
char_to_file(char c)
{
	return (c >= 'a' && c <= 'h') ? c - 'a' : FILE_NONE;
}

Rank
char_to_rank(char c)
{
	return (c >= '1' && c <= '8') ? c - '1' : RANK_NONE;
}

File
square_file(Square sq)
{
	return sq >> 3;
}

Rank
square_rank(Square sq)
{
	return sq & 0x7;
}

Bitboard
square_to_bitboard(Square sq)
{
	return 1ULL << sq;
}

Square
square_new(File f, Rank r)
{
	return f << 3 | r;
}
