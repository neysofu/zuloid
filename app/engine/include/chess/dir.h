/// @file dir.h
/// @brief Enumerator definition for the direction between squares on a
/// chessboard.

#pragma once

#include "coord.h"

/// @brief The direction between any two squares on a chessboard.
enum Dir
{
	/// Two squares on the same rank.
	DIR_HORIZONTAL = 0,
	/// Two squares on the same file.
	DIR_VERTICAL = 1,
	/// Two squares on the same diagonal or anti-diagonal.
	DIR_DIAGONAL = 2,
	/// Two squares a knight's move away from each other.
	DIR_L_SHAPED = 3,
	/// Two squares two queen's moves away from each other.
	DIR_NONE = 4,
};

/// @brief Calculates the direction between any two squares on a chessboard.
/// @pre Both @p coord_0 and @p coord_1 are in bounds.
/// @pre @p coord_0 is \a not equal to @p coord_1.
/// @see coord_is_in_bounds
enum Dir
dir_from_to(Coord coord_0, Coord coord_1);
