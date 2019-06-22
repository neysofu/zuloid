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

#include "chess/movegen.h"
#include "chess/bb.h"
#include "chess/color.h"
#include <stdio.h>
#include <string.h>

enum
{
	MAX_MOVES = 255
};

#define EMIT_MOVE(m, a, b)                                                                 \
	(m)->source = (a);                                                                     \
	(m)->target = (b);                                                                     \
	(m)->promotion = 0;                                                                    \
	(m)++;

#define EMIT_PROMOTION(m, a, b, p)                                                         \
	(m)->source = (a);                                                                     \
	(m)->target = (b);                                                                     \
	(m)->promotion = (p);                                                                  \
	(m)++;

#define EMIT_PROMOTIONS(m, a, b)                                                           \
	EMIT_PROMOTION(m, a, b, PIECE_TYPE_QUEEN)                                              \
	EMIT_PROMOTION(m, a, b, PIECE_TYPE_ROOK)                                               \
	EMIT_PROMOTION(m, a, b, PIECE_TYPE_BISHOP)                                             \
	EMIT_PROMOTION(m, a, b, PIECE_TYPE_KNIGHT)

size_t
gen_pawn_moves(struct Move moves[], struct Position *pos)
{
	struct Move *ptr = moves;
	Bitboard pawns = pos->bb[pos->side_to_move] & pos->bb[PIECE_TYPE_PAWN];
	Bitboard mask =
	  pos->bb[color_other(pos->side_to_move)] | square_to_bb(pos->en_passant_target);
	Bitboard promo = color_promoting_rank(pos->side_to_move);
	Bitboard p1 = (pawns << 8) & ~position_occupancy(pos);
	Bitboard p2 = ((p1 & color_en_passant_target_rank(pos->side_to_move)) << 8) &
	              ~position_occupancy(pos);
	Bitboard a1 = ((pawns & 0xfefefefefefefefeL) << 7) & mask;
	Bitboard a2 = ((pawns & 0x7f7f7f7f7f7f7f7fL) << 9) & mask;
	int sq;
	while (p1) {
		POP_LSB(sq, p1);
		if (BIT(sq) & promo) {
			EMIT_PROMOTIONS(moves, sq - 8, sq);
		} else {
			EMIT_MOVE(moves, sq - 8, sq);
		}
	}
	while (p2) {
		POP_LSB(sq, p2);
		EMIT_MOVE(moves, sq - 16, sq);
	}
	while (a1) {
		POP_LSB(sq, a1);
		if (BIT(sq) & promo) {
			EMIT_PROMOTIONS(moves, sq - 7, sq);
		} else {
			EMIT_MOVE(moves, sq - 7, sq);
		}
	}
	while (a2) {
		POP_LSB(sq, a2);
		if (BIT(sq) & promo) {
			EMIT_PROMOTIONS(moves, sq - 9, sq);
		} else {
			EMIT_MOVE(moves, sq - 9, sq);
		}
	}
	return moves - ptr;
}

size_t
gen_knight_moves(struct Move moves[], Bitboard sources, Bitboard mask)
{
	struct Move *ptr = moves;
	Square source, target;
	while (sources) {
		POP_LSB(source, sources);
		Bitboard targets = BB_KNIGHT[source] & mask;
		while (targets) {
			POP_LSB(target, targets);
			EMIT_MOVE(moves, source, target);
		}
	}
	return moves - ptr;
}

size_t
gen_bishop_moves(struct Move moves[], Bitboard sources, Bitboard mask, Bitboard occupancy)
{
	struct Move *ptr = moves;
	int source, target;
	while (sources) {
		POP_LSB(source, sources);
		Bitboard targets = bb_bishop(source, occupancy) & mask;
		while (targets) {
			POP_LSB(target, targets);
			EMIT_MOVE(moves, source, target);
		}
	}
	return moves - ptr;
}

size_t
gen_rook_moves(struct Move moves[], Bitboard sources, Bitboard mask, Bitboard all)
{
	struct Move *ptr = moves;
	int source, target;
	while (sources) {
		POP_LSB(source, sources);
		Bitboard targets = bb_rook(source, all) & mask;
		while (targets) {
			POP_LSB(target, targets);
			EMIT_MOVE(moves, source, target);
		}
	}
	return moves - ptr;
}

size_t
gen_king_moves(struct Move *moves, Bitboard sources, Bitboard mask)
{
	struct Move *ptr = moves;
	int source, target;
	while (sources) {
		POP_LSB(source, sources);
		Bitboard targets = BB_KING[source] & mask;
		while (targets) {
			POP_LSB(target, targets);
			EMIT_MOVE(moves, source, target);
		}
	}
	return moves - ptr;
}

size_t
gen_king_castles(struct Move moves[], struct Position *pos)
{
	//	struct Move *ptr = moves;
	//	if (pos->castling_rights & CASTLING_RIGHT_KINGSIDE) {
	//		if (!(position_occupancy(pos) & 0x0000000000000060L)) {
	//			struct Move dummy[MAX_MOVES];
	//			Bitboard mask = 0x0000000000000030L;
	//			if (!gen_black_attacks_against(pos, dummy, mask)) {
	//				EMIT_MOVE(moves, 4, 6);
	//			}
	//		}
	//	}
	//	if (pos->castling_rights & CASTLING_RIGHT_QUEENSIDE) {
	//		if (!(position_occupancy(pos) & 0x000000000000000eL)) {
	//			struct Move dummy[MAX_MOVES];
	//			Bitboard mask = 0x0000000000000018L;
	//			if (!gen_black_attacks_against(pos, dummy, mask)) {
	//				EMIT_MOVE(moves, 4, 2);
	//			}
	//		}
	//	}
	//	return moves - ptr;
	return 0;
}

size_t
gen_pseudolegal_moves(struct Move moves[], struct Position *pos)
{
	struct Move *ptr = moves;
	Bitboard pieces = pos->bb[pos->side_to_move];
	// moves += gen_pawn_moves(moves, pos);
	moves += gen_knight_moves(moves, pieces & pos->bb[PIECE_TYPE_KNIGHT], ~pieces);
	moves += gen_bishop_moves(
	  moves, pieces & pos->bb[PIECE_TYPE_BISHOP], ~pieces, position_occupancy(pos));
	moves += gen_rook_moves(
	  moves, pieces & pos->bb[PIECE_TYPE_ROOK], ~pieces, position_occupancy(pos));
	moves += gen_king_moves(moves, pieces, ~pieces);
	// moves += gen_king_castles(moves, pos);
	return moves - ptr;
}

// size_t
// gen_attacks_against_by(struct Position *pos, struct Move *moves, Bitboard mask)
//{
//	struct Move *ptr = moves;
//	moves += gen_white_pawn_attacks_against(pos, moves, mask);
//	moves += gen_white_knight_attacks_against(pos, moves, mask);
//	moves += gen_white_bishop_attacks_against(pos, moves, mask);
//	moves += gen_white_rook_attacks_against(pos, moves, mask);
//	moves += gen_white_queen_attacks_against(pos, moves, mask);
//	moves += gen_white_king_attacks_against(pos, moves, mask);
//	return moves - ptr;
//}
//
// size_t
// gen_attacks(struct Position *pos, struct Move *moves, enum Color color)
//{
//	return gen_attacks_against_by(pos, moves, pos->bb[color_other(color)], color);
//}
//
// size_t
// gen_checks(struct Position *pos, struct Move *moves, enum Color color)
//{
//	return gen_attacks_against_by(
//	  pos, moves, pos->bb[color_other(color)] & pos->bb[PIECE_TYPE_KING], color);
//}
//
// size_t
// gen_pawn_moves(struct Move moves[], struct Position *pos)
//{
//	struct Move *ptr = moves;
//	Bitboard pawns = pos->black_pawns;
//	Bitboard mask = pos->bb[COLOR_WHITE] | pos->ep;
//	Bitboard promo = 0x00000000000000ffL;
//	Bitboard p1 = (pawns >> 8) & ~position_occupancy(pos);
//	Bitboard p2 = ((p1 & 0x0000ff0000000000L) >> 8) & ~position_occupancy(pos);
//	Bitboard a1 = ((pawns & 0x7f7f7f7f7f7f7f7fL) >> 7) & mask;
//	Bitboard a2 = ((pawns & 0xfefefefefefefefeL) >> 9) & mask;
//	int sq;
//	while (p1) {
//		POP_LSB(sq, p1);
//		if (BIT(sq) & promo) {
//			EMIT_PROMOTIONS(moves, sq + 8, sq);
//		} else {
//			EMIT_MOVE(moves, sq + 8, sq);
//		}
//	}
//	while (p2) {
//		POP_LSB(sq, p2);
//		EMIT_MOVE(moves, sq + 16, sq);
//	}
//	while (a1) {
//		POP_LSB(sq, a1);
//		if (BIT(sq) & promo) {
//			EMIT_PROMOTIONS(moves, sq + 7, sq);
//		} else {
//			EMIT_MOVE(moves, sq + 7, sq);
//		}
//	}
//	while (a2) {
//		POP_LSB(sq, a2);
//		if (BIT(sq) & promo) {
//			EMIT_PROMOTIONS(moves, sq + 9, sq);
//		} else {
//			EMIT_MOVE(moves, sq + 9, sq);
//		}
//	}
//	return moves - ptr;
//}
//
//// black attack generators
// size_t
// gen_black_pawn_attacks_against(struct Position *pos, struct Move *moves, Bitboard mask)
//{
//	struct Move *ptr = moves;
//	Bitboard pawns = pos->black_pawns;
//	Bitboard a1 = ((pawns & 0x7f7f7f7f7f7f7f7fL) >> 7) & mask;
//	Bitboard a2 = ((pawns & 0xfefefefefefefefeL) >> 9) & mask;
//	int sq;
//	while (a1) {
//		POP_LSB(sq, a1);
//		EMIT_MOVE(moves, sq + 7, sq);
//	}
//	while (a2) {
//		POP_LSB(sq, a2);
//		EMIT_MOVE(moves, sq + 9, sq);
//	}
//	return moves - ptr;
//}
//
// size_t
// gen_pseudolegal_moves(struct Move moves[], struct Position *pos)
//{
//	if (pos->side_to_move) {
//		return gen_black_moves(pos, moves);
//	} else {
//		return gen_white_moves(pos, moves);
//	}
//}
//
// size_t
// gen_legal_moves(struct Move moves[], struct Position *pos)
//{
//	struct Move *ptr = moves;
//	struct Move temp[MAX_MOVES];
//	int count = gen_moves(pos, temp);
//	for (size_t i = 0; i < count; i++) {
//		struct Move *move = &temp[i];
//		position_do_move(pos, move);
//		if (!is_illegal(pos)) {
//			memcpy(moves++, move, sizeof(struct Move));
//		}
//		position_undo_move(pos, move);
//	}
//	return moves - ptr;
//}
//
// size_t
// gen_attacks(struct Move moves[], struct Position *pos)
//{
//	if (pos->side_to_move) {
//		return gen_black_attacks(pos, moves);
//	} else {
//		return gen_white_attacks(pos, moves);
//	}
//}
//
// int
// is_check(struct Position *pos)
//{
//	struct Move moves[MAX_MOVES];
//	if (pos->side_to_move) {
//		return gen_white_checks(pos, moves);
//	} else {
//		return gen_black_checks(pos, moves);
//	}
//}
//
// int
// is_illegal(struct Position *pos)
//{
//	struct Move moves[MAX_MOVES];
//	return gen_checks(pos, moves, pos->side_to_move);
//}
//
// int
// has_legal_moves(struct Position *pos)
//{
//	struct Move moves[MAX_MOVES];
//	return gen_legal_moves(pos, moves);
//}
