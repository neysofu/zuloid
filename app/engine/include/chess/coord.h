/// @file coord.h
/// @brief Chess coordinates.

#pragma once

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>

typedef uint_fast8_t File;
typedef uint_fast8_t Rank;
/// @brief A little-endian reprsentation of chess coordinates.
/// Every value between 0
/// (a1) and 63 (h8) points to a square on the chessboard.
typedef uint_fast8_t Coord;

/// @defgroup file Files
/// @{

/// @brief Returns this file's lowercase letter label.
/// @param file The file.
/// @return the lowercase letter label of @p file.
/// @pre @p file is in bounds.
/// @see @c file_is_in_bounds
char
file_to_char(File file);

/// Returns the file labelled with this letter, or FILE_NONE if none.
///
/// @return The file labelled with the letter @c (either case), or @c FILE_NONE
/// if none.
File
char_to_file(char c);

/// @}

/// @defgroup rank Ranks
/// @{

/// @brief Returns @p rank's digit label.
/// @param rank The chessboard rank to look up.
/// @pre @p rank is in bounds.
/// @see @c rank_is_in_bounds
char
rank_to_char(Rank rank);

// Returns the rank labelled with this digit, or RANK_NONE is none.
Rank
char_to_rank(char c);

/// @}

/// @defgroup coord Coordinates
/// @{


// Returns the square specified by a file and a rank.
Coord
coord_new(File file, Rank rank);

bool
coord_is_in_bounds(Coord coord);

// Returns this square's file.
File
coord_file(Coord coord);

// Returns this square's rank.
Rank
coord_rank(Coord coord);

/// @}

// Out-of-bounds dummy values.
extern const File FILE_NONE;
extern const Rank RANK_NONE;
extern const Coord COORD_NONE;
