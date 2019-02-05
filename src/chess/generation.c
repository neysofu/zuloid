/* Copyright (C) 2014 Michael Fogleman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chess/generation.h"

const size_t MAX_MOVES_COUNT = 256;

////#define EMIT_MOVE(m, a, b)                                                                 \
//	(m)->src = (a);                                                                        \
//	(m)->dst = (b);                                                                        \
//	(m)->promotion = EMPTY;                                                                \
//	(m)++;
//
//#define EMIT_PROMOTION(m, a, b, p)                                                         \
//	(m)->src = (a);                                                                        \
//	(m)->dst = (b);                                                                        \
//	(m)->promotion = (p);                                                                  \
//	(m)++;
//
//#define EMIT_PROMOTIONS(m, a, b)                                                           \
//	EMIT_PROMOTION(m, a, b, QUEEN)                                                         \
//	EMIT_PROMOTION(m, a, b, ROOK)                                                          \
//	EMIT_PROMOTION(m, a, b, BISHOP)                                                        \
//	EMIT_PROMOTION(m, a, b, KNIGHT)
//
//// generic move generators
//int
//gen_knight_moves(Square square, Bitboard mask, Moves moves[])
//{
//	size_t moves_count = 0;
//	int src, dst;
//	while (srcs) {
//		POP_LSB(src, srcs);
//		Bitboard attacks = piece_knight_attacks(source) & mask;
//		while (dsts) {
//			POP_LSB(dst, dsts);
//			EMIT_MOVE(moves, src, dst);
//		}
//	}
//	return moves_count;
//}
//
//// white move generators
//int
//gen_white_pawn_moves(struct Position *position, Move *moves)
//{
//	Move *ptr = moves;
//	Bitboard pawns = position->occupancy_by_piece_type[PIECE_TYPE_PAWN] &
//	                 position->occupancy_by_color[COLOR_WHITE];
//	Bitboard mask = position->occupancy_by_color[COLOR_BLACK] | position->ep;
//	Bitboard promotion = color_promoting_rank(position->side_to_move);
//	Bitboard p1 = (pawns << 8) & ~position->all;
//	Bitboard p2 = ((p1 & 0x0000000000ff0000L) << 8) & ~position->all;
//	Bitboard a1 = ((pawns & 0xfefefefefefefefeL) << 7) & mask;
//	Bitboard a2 = ((pawns & 0x7f7f7f7f7f7f7f7fL) << 9) & mask;
//	Square square;
//	while (p1) {
//		POP_LSB(square, p1);
//		if (BIT(sq) & promo) {
//			EMIT_PROMOTIONS(moves, square - 8, sq);
//		} else {
//			EMIT_MOVE(moves, square - 8, sq);
//		}
//	}
//	while (p2) {
//		POP_LSB(square, p2);
//		EMIT_MOVE(moves, square - 16, sq);
//	}
//	while (a1) {
//		POP_LSB(square, a1);
//		if (BIT(sq) & promo) {
//			EMIT_PROMOTIONS(moves, square - 7, sq);
//		} else {
//			EMIT_MOVE(moves, square - 7, sq);
//		}
//	}
//	while (a2) {
//		POP_LSB(square, a2);
//		if (BIT(sq) & promo) {
//			EMIT_PROMOTIONS(moves, square - 9, sq);
//		} else {
//			EMIT_MOVE(moves, square - 9, sq);
//		}
//	}
//	return moves - ptr;
//}
//
//int
//gen_white_king_castles(struct Position *position, Move *moves)
//{
//	Move *ptr = moves;
//	if (position->castle & CASTLE_WHITE_KING) {
//		if (!(position->all & 0x0000000000000060L)) {
//			Move dummy[MAX_MOVES];
//			Bitboard mask = 0x0000000000000030L;
//			if (!gen_black_attacks_against(position, dummy, mask)) {
//				EMIT_MOVE(moves, 4, 6);
//			}
//		}
//	}
//	if (position->castle & CASTLE_WHITE_QUEEN) {
//		if (!(position->all & 0x000000000000000eL)) {
//			Move dummy[MAX_MOVES];
//			Bitboard mask = 0x0000000000000018L;
//			if (!gen_black_attacks_against(position, dummy, mask)) {
//				EMIT_MOVE(moves, 4, 2);
//			}
//		}
//	}
//	return moves - ptr;
//}
//
//int
//position_generate_moves(struct Position *position, Move *moves)
//{
//	return gen_white_pawn_moves(position, moves) + gen_white_knight_moves(position, moves) +
//	       gen_white_bishop_moves(position, moves) + gen_white_rook_moves(position, moves) +
//	       gen_white_queen_moves(position, moves) + gen_white_king_moves(position, moves) +
//	       gen_white_king_castles(position, moves);
//}
//
//// white attack generators
//int
//gen_white_pawn_attacks_against(struct Position *position, Move *moves, Bitboard mask)
//{
//	Move *ptr = moves;
//	Bitboard pawns = position->white_pawns;
//	Bitboard a1 = ((pawns & 0xfefefefefefefefeL) << 7) & mask;
//	Bitboard a2 = ((pawns & 0x7f7f7f7f7f7f7f7fL) << 9) & mask;
//	Square square;
//	while (a1) {
//		POP_LSB(square, a1);
//		EMIT_MOVE(moves, square - 7, sq);
//	}
//	while (a2) {
//		POP_LSB(square, a2);
//		EMIT_MOVE(moves, square - 9, sq);
//	}
//	return moves - ptr;
//}
//
//int
//gen_white_attacks_against(struct Position *position, Move *moves, Bitboard mask)
//{
//	Move *ptr = moves;
//	moves += gen_white_pawn_attacks_against(position, moves, mask);
//	moves += gen_white_knight_attacks_against(position, moves, mask);
//	moves += gen_white_bishop_attacks_against(position, moves, mask);
//	moves += gen_white_rook_attacks_against(position, moves, mask);
//	moves += gen_white_queen_attacks_against(position, moves, mask);
//	moves += gen_white_king_attacks_against(position, moves, mask);
//	return moves - ptr;
//}
//
//int
//gen_white_attacks(struct Position *position, Move *moves)
//{
//	return gen_white_attacks_against(position, moves, position->black);
//}
//
//int
//gen_white_checks(struct Position *position, Move *moves)
//{
//	return gen_white_attacks_against(position, moves, position->black_kings);
//}
//int
//gen_black_pawn_attacks_against(struct Position *position, Move *moves, Bitboard mask)
//{
//	Move *ptr = moves;
//	Bitboard pawns = position->black_pawns;
//	Bitboard a1 = ((pawns & 0x7f7f7f7f7f7f7f7fL) >> 7) & mask;
//	Bitboard a2 = ((pawns & 0xfefefefefefefefeL) >> 9) & mask;
//	Square square;
//	while (a1) {
//		POP_LSB(square, a1);
//		EMIT_MOVE(moves, square + 7, sq);
//	}
//	while (a2) {
//		POP_LSB(square, a2);
//		EMIT_MOVE(moves, square + 9, sq);
//	}
//	return moves - ptr;
//}
//
//// color determined by position
//int
//position_generate_legal_moves(struct Position *position, Move moves[])
//{
//	switch (position->side_to_move) {
//		case COLOR_WHITE:
//			return gen_black_moves(position, moves);
//		case COLOR_BLACK:
//			return gen_white_moves(position, moves);
//		default:
//			return 0;
//	}
//}
//
//int
//position_gen_legal_moves(struct Position *position, Move *moves)
//{
//	struct MoveUndo undo;
//	struct Move moves[MAX_MOVES_COUNT];
//	size_t moves_count = position_gen_moves(position, moves);
//	for (size_t i = 0; i < moves_count; i++) {
//		Move *move = &temp[i];
//		do_move(position, move, &undo);
//		if (!position_is_legal(position)) {
//			memcpy(moves++, move, sizeof(Move));
//		}
//		undo_move(position, move, &undo);
//	}
//	return moves - ptr;
//}
//
//int
//position_gen_attacks(struct Position *position, Move *moves)
//{
//	if (position->color) {
//		return gen_black_attacks(position, moves);
//	} else {
//		return gen_white_attacks(position, moves);
//	}
//}
//
//int
//position_gen_checks(struct Position *position, Move *moves)
//{
//	if (position->color) {
//		return gen_black_checks(position, moves);
//	} else {
//		return gen_white_checks(position, moves);
//	}
//}
//
//int
//position_is_check(struct Position *position)
//{
//	Move moves[MAX_MOVES];
//	if (position->color) {
//		return gen_white_checks(position, moves);
//	} else {
//		return gen_black_checks(position, moves);
//	}
//}
//
//int
//position_is_legal(struct Position *position)
//{
//	Move moves[MAX_MOVES];
//	if (position->color) {
//		return position_attacks_against(position, moves) == 0;
//	} else {
//		return position_attacks_against(position, moves) == 0;
//	}
//}

size_t
position_gen_pseudolegal_moves(const struct Position *position, struct Move buffer[])
{
	return 0;
}

size_t
position_gen_legal_moves(const struct Position *position, struct Move buffer[]);
