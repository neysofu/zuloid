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
	//	Bitboard promo = color_promoting_rank(pos->side_to_move);
	//	Bitboard p1 = (pawns << 8) & ~position_occupancy(pos);
	//	Bitboard p2 = ((p1 & color_en_passant_target_rank(pos->side_to_move)) << 8) &
	//	              ~position_occupancy(pos);
	//	Bitboard a1 = ((pawns & 0xfefefefefefefefeL) << 7) & mask;
	//	Bitboard a2 = ((pawns & 0x7f7f7f7f7f7f7f7fL) << 9) & mask;
	//	int sq;
	//	while (sources) {
	//		POP_LSB(sq, p1);
	//		if (BIT(sq) & promo) {
	//			EMIT_PROMOTIONS(moves, sq - 8, sq);
	//		} else {
	//			EMIT_MOVE(moves, sq - 8, sq);
	//		}
	//	}
	//	while (p2) {
	//		POP_LSB(sq, p2);
	//		EMIT_MOVE(moves, sq - 16, sq);
	//	}
	//	while (a1) {
	//		POP_LSB(sq, a1);
	//		if (BIT(sq) & promo) {
	//			EMIT_PROMOTIONS(moves, sq - 7, sq);
	//		} else {
	//			EMIT_MOVE(moves, sq - 7, sq);
	//		}
	//	}
	//	while (a2) {
	//		POP_LSB(sq, a2);
	//		if (BIT(sq) & promo) {
	//			EMIT_PROMOTIONS(moves, sq - 9, sq);
	//		} else {
	//			EMIT_MOVE(moves, sq - 9, sq);
	//		}
	//	}
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
	struct Move *ptr = moves;
	Bitboard mask, maskk, foo;
	/* TODO */
	if (pos->castling_rights & (CASTLING_RIGHT_KINGSIDE << pos->side_to_move)) {
		mask = position_castle_mask(pos, CASTLING_RIGHT_KINGSIDE);
		position_flip_side_to_move(pos);
		maskk = mask;
		POP_LSB(foo, maskk);
		POP_MSB(foo, maskk);
		if (!(position_occupancy(pos) & maskk)) {
			// struct Move dummy[255];
			// if (!gen_attacks_against(dummy, pos, mask)) {
			EMIT_MOVE(moves,
			          bb_to_square(pos->bb[PIECE_TYPE_KING] & pos->bb[pos->side_to_move]),
			          square_new(6, color_home_rank(pos->side_to_move)));
			//}
		}
		position_flip_side_to_move(pos);
	}
	if (pos->castling_rights & (CASTLING_RIGHT_QUEENSIDE << pos->side_to_move)) {
		position_flip_side_to_move(pos);
		mask = position_castle_mask(pos, CASTLING_RIGHT_QUEENSIDE);
		maskk = mask;
		POP_LSB(foo, maskk);
		POP_MSB(foo, maskk);
		if (!(position_occupancy(pos) & maskk)) {
			// struct Move dummy[255];
			// if (!gen_attacks_against(dummy, pos, mask)) {
			EMIT_MOVE(moves,
			          bb_to_square(pos->bb[PIECE_TYPE_KING] & pos->bb[pos->side_to_move]),
			          square_new(2, color_home_rank(pos->side_to_move)));
			//}
		}
		position_flip_side_to_move(pos);
	}
	return moves - ptr;
}

size_t
gen_pseudolegal_moves(struct Move moves[], struct Position *pos)
{
	return gen_attacks_against(moves, pos, ~pos->bb[pos->side_to_move]);
}

bool
position_is_illegal(struct Position *pos)
{
	struct Move moves[MAX_MOVES];
	return gen_checks_to(moves, pos, pos->side_to_move);
}

size_t
gen_legal_moves(struct Move moves[], struct Position *pos)
{
	struct Move *ptr = moves;
	struct Move temp[MAX_MOVES];
	size_t count = gen_pseudolegal_moves(temp, pos);
	for (size_t i = 0; i < count; i++) {
		struct Move *move = temp + i;
		position_do_move(pos, move);
		position_flip_side_to_move(pos);
		if (!position_is_illegal(pos)) {
			memcpy(moves++, move, sizeof(struct Move));
		}
		position_flip_side_to_move(pos);
		position_undo_move(pos, move);
	}
	return moves - ptr;
}

size_t
gen_attacks_against(struct Move moves[], struct Position *pos, Bitboard victims)
{
	struct Move *ptr = moves;
	Bitboard pieces = pos->bb[pos->side_to_move];
	moves += gen_pawn_moves(moves, pos);
	moves += gen_knight_moves(moves, pieces & pos->bb[PIECE_TYPE_KNIGHT], victims);
	moves += gen_bishop_moves(
	  moves, pieces & pos->bb[PIECE_TYPE_BISHOP], victims, position_occupancy(pos));
	moves += gen_rook_moves(
	  moves, pieces & pos->bb[PIECE_TYPE_ROOK], victims, position_occupancy(pos));
	moves += gen_king_moves(moves, pieces & pos->bb[PIECE_TYPE_KING], victims);
	moves += gen_king_castles(moves, pos);
	return moves - ptr;
}

size_t
gen_checks_to(struct Move moves[], struct Position *pos, enum Color color)
{
	return gen_attacks_against(moves, pos, pos->bb[color] & pos->bb[PIECE_TYPE_KING]);
}

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

bool
position_is_stalemate(struct Position *pos)
{
	struct Move moves[MAX_MOVES];
	return gen_legal_moves(pos, moves) == 0;
}
