/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This file provides bitboard utilities for the `uint64_t` data type.
 *
 * Resources
 *   https://www.chessprogramming.org/Bitboards
 *   https://www.chessprogramming.org/General_Setwise_Operations
 *   https://en.wikipedia.org/wiki/Bitboard
 *   https://graphics.stanford.edu/~seander/bithacks.html
 *   http://cinnamonchess.altervista.org/bitboard_calculator/Calc.html */

#pragma once

#include "chess/coord.h"
#include "chess/dir.h"
#include "chess/move.h"
#include <stdint.h>

/* Initializes bitboard lookup tables. Do NOT call any of the other 'bb'
 * functions without first calling 'bb_init' at the start of your program (it
 * will cause undefined behaviour)! This
 * function can safely be called multiple times without any effect after
 * the first call.
 *
 * Effects
 *   After initialization, all other `bb_` are safe to call. */
void
bb_init(void);

/* Masks this square. */
uint64_t
bb_coord(Coord coord);

/* Masks all squares on the same file as `coord`. */
uint64_t
bb_file(Coord coord);

/// Masks the files directly adjacent to this square's file.
/// @pre Initialization via `bb_init`.
// uint64_t
// bb_adjiacent_files(Coord coord);

/// Masks this square's rank.
/// @pre Initialization via `bb_init`.
uint64_t
bb_rank(Coord coord);

/// Masks the ranks directly adjacent to this square's rank.
/// @pre Initialization via `bb_init`.
// uint64_t
// bb_adjiacent_ranks(Coord coord);

/// @brief Masks the squares involved in a sliding move.
/// @pre Initialization via `bb_init`.
uint64_t
bb_ray(Move move);

/* Masks the threats that a certain piece will make from `coord`. */
uint64_t
bb_knight_threats(Coord coord);
uint64_t
bb_bishop_threats(Coord coord);
uint64_t
bb_rook_threats(Coord coord);
uint64_t
bb_queen_threats(Coord coord);
uint64_t
bb_king_threats(Coord coord);
