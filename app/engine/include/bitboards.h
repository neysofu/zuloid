/// @file bitboards.h
/// @brief Bitboard masking utilities.
///
/// @see https://www.chessprogramming.org/Bitboards
/// @see https://www.chessprogramming.org/General_Setwise_Operations
/// @see https://en.wikipedia.org/wiki/Bitboard
/// @see http://cinnamonchess.altervista.org/bitboard_calculator/Calc.html

#pragma once

#include <stdint.h>
#include "chess/coord.h"
#include "chess/dir.h"
#include "chess/move.h"

// Initializes bitboard lookup tables. Do NOT call any of the other 'bb'
// functions without first calling 'bb_init' at the start of your program (it
// will cause undefined behaviour)! This
// function can safely be called multiple times without any effect after
// the first call.
///
/// @post Any of the other functions prototyped in this file can be
/// called safely.
void
bb_init(void);

/// @brief Masks this square.
///
/// @return A bitboard that masks only the bit at this coordinates.
///
/// @pre Initialization via `bb_init`.
uint64_t
bb_coord(Coord coord);

// Masks this square's file.
/// @pre Initialization via `bb_init`.
uint64_t
bb_file(Coord coord);

/// Masks the files directly adjacent to this square's file.
/// @pre Initialization via `bb_init`.
//uint64_t
//bb_adjiacent_files(Coord coord);

/// Masks this square's rank.
/// @pre Initialization via `bb_init`.
uint64_t
bb_rank(Coord coord);

/// Masks the ranks directly adjacent to this square's rank.
/// @pre Initialization via `bb_init`.
//uint64_t
//bb_adjiacent_ranks(Coord coord);

/// @brief Masks this square's diagonals.
///
/// @par coord_0 The first coordinates.
/// @par coord_1 The first coordinates.
/// @par dir The direction between @p coord_0 and @p coord_1 as computed by
// `dir(coord_0, coord_1)`.
/// @pre Initialization via `bb_init`.
/// @pre @p coord is valid chess coordinates.
/// @see https://www.chessprogramming.org/Diagonals
uint64_t
bb_diagonals(Coord coord);

/// @brief Masks the squares involved in a sliding move.
/// @pre Initialization via `bb_init`.
uint64_t
bb_ray(Move move);

/// Masks the threats of specific piece types from this square.
/// @pre Initialization via `bb_init`.
uint64_t
bb_knight_threats(Coord coord);
uint64_t
bb_bishop_threats(Coord coord);
uint64_t
bb_rook_threats(Coord coord);
uint64_t
bb_king_attacks(Coord coord);
uint64_t
bb_queen_attacks(Coord coord);
