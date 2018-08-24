#pragma once

#include <stdint.h>
#include "coord.h"

// Initialize bitboards in lookup tables. Execution is extremely fast, so time
// is not an issue.
void
bb_init(void);

// Mask a square's file.
uint64_t
bb_file(Coord coord);

// Mask the files directly adjiacten to a square's.
uint64_t
bb_adjiacent_files(Coord coord);

// Mask a square's rank.
uint64_t
bb_rank(Coord coord);

// Mask the ranks directly adjiacten to a square's.
uint64_t
bb_adjiacent_ranks(Coord coord);

// Mask the main diagonal (a1 to h8) going through 'coord'.
uint64_t
bb_diagonals_main(Coord coord);

// Main the anti-diagonal (a8 to h1) going through 'coord'.
uint64_t
bb_diagonals_anti(Coord coord);

// Mask the non-capture moves available to a pawn.
uint64_t
bb_pawn_pushes(Coord coord);

// Mask the target squares for pawns by a double push.
uint64_t
bb_pawn_double_pushes(Coord coord);

// Mask the attack rays for a rook.
uint64_t
bb_rook_attacks(Coord coord);

// Mask the squares threatened by a knight.
uint64_t
bb_knight_attacks(Coord coord);

// Mask the squares threatened by a bishop.
uint64_t
bb_bishop_attacks(Coord coord);

// Mask the squares threatened by a king.
uint64_t
bb_king_attacks(Coord coord);
