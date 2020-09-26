#ifndef ZULOID_CHESS_MOVEGEN_H
#define ZULOID_CHESS_MOVEGEN_H

#include "chess/color.h"
#include "chess/coordinates.h"
#include "chess/move.h"
#include "chess/position.h"
#include <stdbool.h>
#include <stdio.h>

enum
{
	MAX_MOVES = 276
};

size_t
gen_pawn_moves(struct Move moves[],
               Bitboard srcs,
               Bitboard mask,
               Bitboard all,
               Bitboard en_passant,
               enum Color side_to_move);
size_t
gen_knight_moves(struct Move moves[], Bitboard srcs, Bitboard mask);
size_t
gen_bishop_moves(struct Move moves[], Bitboard srcs, Bitboard mask, Bitboard all);
size_t
gen_rook_moves(struct Move moves[], Bitboard srcs, Bitboard mask, Bitboard all);
size_t
gen_king_moves(struct Move moves[], Bitboard srcs, Bitboard mask);
size_t
gen_queen_moves(struct Move moves[], Bitboard srcs, Bitboard mask, Bitboard all);

size_t
gen_pseudolegal_moves(struct Move moves[], struct Board *pos);
size_t
gen_legal_moves(struct Move moves[], struct Board *pos);
size_t
gen_attacks_against_from(struct Move moves[],
                         struct Board *pos,
                         Bitboard victims,
                         enum Color attacker,
                         Square en_passant_target,
                         bool castle);
bool
position_is_illegal(struct Board *pos);
bool
position_is_stalemate(struct Board *pos);

#endif
