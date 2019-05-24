/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chess/coordinates.h"
#include "chess/color.h"

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
square_file(Square square)
{
	return square >> 3;
}

Rank
square_rank(Square square)
{
	return square & 0x7;
}

Bitboard
square_to_bitboard(Square square)
{
	return 1ULL << square;
}

Square
square_new(File file, Rank rank)
{
	return file << 3 | rank;
}
