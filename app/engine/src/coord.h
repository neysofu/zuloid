#pragma once

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include "dir.h"

// Integer representations for the chess concepts of files, ranks, and
// coordinates.
typedef uint_fast8_t File;
typedef uint_fast8_t Rank;
typedef uint_fast8_t Coord;

// Returns the lowercase letter conventionally used to represent 'file'.
char
file_to_char(File file);

// Returns the file conventionally represented by the lowercase letter 'c'.
File
char_to_file(char c);

// Checks wheather or not a file is within the bounds of a chessboard (8x8).
bool
file_is_valid(File file);

// Returns the digit conventionally used to represent 'rank'.
char
rank_to_char(Rank rank);

// Returns the rank conventionally represented by the digit 'c'.
Rank
char_to_rank(char c);

// Checks wheather or not a file is within the bounds of a chessboard (8x8).
bool
rank_is_valid(Rank rank);

// Returns the coordinates specified by 'file' and 'rank'.
Coord
coord_new(File file, Rank rank);

// Returns the bitmask that queries the bit set at 'coord'.
uint64_t
coord_to_bb(Coord coord);

// Returns the file component of 'coord'.
File
coord_file(Coord coord);

// Returns the rank component of 'coord'.
Rank
coord_rank(Coord coord);

// Calls 'file_to_char' and 'rank_to_char' on the first two characters of
// 'str'. (allocating it if set to NULL). The string will NOT be
// NULL-terminated.
void
coord_to_str(Coord coord, char *str);

// Reads a coordinate from 'str' as if written by 'coord_to_str' and returns it.
// Please note that 'str' must contain at least two characters.
Coord
str_to_coord(char *str);

// Checks wheater 'coord' is within the bounds of a 8x8 chessboard.
bool
coord_is_valid(Coord coord);

// Computes and returns the direction between two coordinates 'coord1' and
// 'coord2'. Please note that:
// * coord_dir(x, y) == coord_dir(y, x)
// * coord_dir(x, x) == COORD_NONE
enum Dir
coord_dir(Coord coord1, Coord coord2);

// The following are some out-of-bounds values.
extern const File FILE_NONE;
extern const Rank RANK_NONE;
extern const Coord COORD_NONE;
