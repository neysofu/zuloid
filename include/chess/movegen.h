/* Copyright (C) 2014 Michael Fogleman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, and to permit
 * persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE. */

#ifndef ZORRO_CHESS_MOVEGEN_H
#define ZORRO_CHESS_MOVEGEN_H

#include "chess/color.h"
#include "chess/coordinates.h"
#include "chess/move.h"
#include "chess/position.h"

enum
{
	MAX_MOVES = 276
};

size_t
gen_pawn_moves(struct Move moves[],
               Bitboard srcs,
               Bitboard mask,
               Bitboard all,
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
gen_pseudolegal_moves(struct Move moves[], struct Position *pos);
size_t
gen_legal_moves(struct Move moves[], struct Position *pos);
size_t
gen_attacks_against_from(struct Move moves[],
                         struct Position *pos,
                         Bitboard victims,
                         enum Color attacker);
bool
position_is_illegal(struct Position *pos);
bool
position_is_stalemate(struct Position *pos);

size_t
position_perft(struct Position *pos, size_t depth);

#endif
