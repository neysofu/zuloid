/* Copyright (C) 2014 Michael Fogleman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE. */

#include "chess/movegen.h"
#include "chess/bb.h"
#include "chess/color.h"
#include "chess/fen.h"
#include "chess/magic.h"
#include "chess/mnemonics.h"
#include "chess/position.h"
#include "chess/threats.h"
#include "meta.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define EMIT_MOVE(m, a, b)                                                                 \
	(m)->source = (a);                                                                     \
	(m)->target = (b);                                                                     \
	(m)->promotion = 0;                                                                    \
	(m)++;

void
emit_move(struct Move *move, Square source, Square target)
{
	move->source = source;
	move->target = target;
	move->promotion = PIECE_TYPE_NONE;
}

/* Generates all pseudolegal moves by pawns located on 'sources' to 'targets'
 * target squares. 'all' gives information regarding piece occupancy for
 * determining pawn pushes and captures. Finally, 'side_to_move' determines in
 * which direction pawn moves happen. */
size_t
gen_pawn_moves(struct Move moves[],
               Bitboard sources,
               Bitboard targets,
               Bitboard all,
               Bitboard en_passant_target,
               enum Color side_to_move)
{
	struct Move *ptr = moves;
	Bitboard single_pushes =
	  (side_to_move == COLOR_WHITE ? sources << 1 : sources >> 1) & ~all;
	Bitboard double_pushes =
	  (side_to_move == COLOR_WHITE ? single_pushes << 1 : single_pushes >> 1) & ~all &
	  targets & rank_to_bb(color_double_push_rank(side_to_move));
	single_pushes &= targets;
	const int color_params[4][COLORS_COUNT] = {
		{ -1, 1 },
		{ -2, 2 },
		{ -9, -7 },
		{ 7, 9 },
	};
	Bitboard captures_east = sources & ~file_to_bb(F_H);
	Bitboard captures_west = sources & ~file_to_bb(F_A);
	const Bitboard captures_params[COLORS_COUNT][2] = {
		[COLOR_WHITE] = { captures_east << 9, captures_west >> 7 },
		[COLOR_BLACK] = { captures_east << 7, captures_west >> 9 }
	};
	captures_east =
	  captures_params[side_to_move][0] & ((targets & all) | en_passant_target);
	captures_west =
	  captures_params[side_to_move][1] & ((targets & all) | en_passant_target);
	Square square;
	while (single_pushes) {
		POP_LSB(square, single_pushes);
		emit_move(moves, square + color_params[0][side_to_move], square);
		if (square_rank(square) == color_promoting_rank(side_to_move)) {
			moves->promotion = PIECE_TYPE_QUEEN;
		}
		moves++;
	}
	while (double_pushes) {
		POP_LSB(square, double_pushes);
		emit_move(moves++, square + color_params[1][side_to_move], square);
	}
	while (captures_east) {
		POP_LSB(square, captures_east);
		emit_move(moves, square + color_params[2][side_to_move], square);
		if (square_rank(square) == color_promoting_rank(side_to_move)) {
			moves->promotion = PIECE_TYPE_QUEEN;
		}
		moves++;
	}
	while (captures_west) {
		POP_LSB(square, captures_west);
		emit_move(moves, square + color_params[3][side_to_move], square);
		if (square_rank(square) == color_promoting_rank(side_to_move)) {
			moves->promotion = PIECE_TYPE_QUEEN;
		}
		moves++;
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
		Bitboard targets = threats_by_knight(source) & mask;
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
		Bitboard targets = threats_by_bishop(source, occupancy) & mask;
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
		Bitboard targets = threats_by_rook(source, all) & mask;
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
		Bitboard targets = threats_by_king(source) & mask;
		while (targets) {
			POP_LSB(target, targets);
			EMIT_MOVE(moves, source, target);
		}
	}
	return moves - ptr;
}

size_t
gen_king_castles(struct Move moves[], struct Board *pos)
{
	struct Move *ptr = moves;
	if (pos->castling_rights & (CASTLING_RIGHT_KINGSIDE << pos->side_to_move)) {
		Bitboard mask = position_castle_mask(pos, CASTLING_RIGHT_KINGSIDE);
		Bitboard king = pos->bb[PIECE_TYPE_KING] & pos->bb[pos->side_to_move];
		if (!(position_occupancy(pos) & (mask ^ king))) {
			Square source = bb_to_square(king);
			EMIT_MOVE(moves, source, source + 16);
		}
	}
	if (pos->castling_rights & (CASTLING_RIGHT_QUEENSIDE << pos->side_to_move)) {
		Bitboard mask = position_castle_mask(pos, CASTLING_RIGHT_QUEENSIDE);
		Bitboard king = pos->bb[PIECE_TYPE_KING] & pos->bb[pos->side_to_move];
		if (!(position_occupancy(pos) & (mask ^ king))) {
			Square source = bb_to_square(king);
			EMIT_MOVE(moves, source, source - 16);
		}
	}
	// if (pos->castling_rights & (CASTLING_RIGHT_QUEENSIDE << pos->side_to_move)) {
	//	position_flip_side_to_move(pos);
	//	mask = position_castle_mask(pos, CASTLING_RIGHT_QUEENSIDE);
	//	maskk = mask;
	//	POP_LSB(foo, maskk);
	//	POP_MSB(foo, maskk);
	//	if (!(position_occupancy(pos) & maskk)) {
	//		// struct Move dummy[255];
	//		// if (!gen_attacks_against(dummy, pos, mask)) {
	//		EMIT_MOVE(moves,
	//		          bb_to_square(pos->bb[PIECE_TYPE_KING] & pos->bb[pos->side_to_move]),
	//		          square_new(2, color_home_rank(pos->side_to_move)));
	//		//}
	//	}
	//	position_flip_side_to_move(pos);
	//}
	return moves - ptr;
}

size_t
gen_attacks_against_from(struct Move moves[],
                         struct Board *pos,
                         Bitboard victims,
                         enum Color attacker,
                         Square en_passant_target,
                         bool castle)
{
	struct Move *ptr = moves;
	Bitboard pieces = pos->bb[attacker];
	moves += gen_pawn_moves(moves,
	                        pieces & pos->bb[PIECE_TYPE_PAWN],
	                        victims,
	                        position_occupancy(pos),
	                        0, // square_to_bb(en_passant_target),
	                        attacker);
	moves += gen_knight_moves(moves, pieces & pos->bb[PIECE_TYPE_KNIGHT], victims);
	moves += gen_bishop_moves(
	  moves, pieces & pos->bb[PIECE_TYPE_BISHOP], victims, position_occupancy(pos));
	moves += gen_rook_moves(
	  moves, pieces & pos->bb[PIECE_TYPE_ROOK], victims, position_occupancy(pos));
	moves += gen_king_moves(moves, pieces & pos->bb[PIECE_TYPE_KING], victims);
	if (castle) {
		moves += gen_king_castles(moves, pos);
	}
	return moves - ptr;
}

size_t
gen_pseudolegal_moves(struct Move moves[], struct Board *pos)
{
	return gen_attacks_against_from(moves,
	                                pos,
	                                ~pos->bb[pos->side_to_move],
	                                pos->side_to_move,
	                                pos->en_passant_target,
	                                true);
}

bool
position_is_illegal(struct Board *pos)
{
	struct Move moves[MAX_MOVES];
	return gen_attacks_against_from(moves,
	                                pos,
	                                pos->bb[color_other(pos->side_to_move)] &
	                                  pos->bb[PIECE_TYPE_KING],
	                                pos->side_to_move,
	                                pos->en_passant_target,
	                                false);
}

size_t
gen_legal_moves(struct Move moves[], struct Board *pos)
{
	int i = 0;
	int count = gen_pseudolegal_moves(moves, pos);
	while (i < count) {
		position_do_move_and_flip(pos, moves + i);
		if (position_is_illegal(pos)) {
			position_undo_move_and_flip(pos, moves + i);
			moves[i] = moves[--count];
		} else {
			position_undo_move_and_flip(pos, moves + i);
			i++;
		}
	}
	return i;
}

bool
position_is_stalemate(struct Board *pos)
{
	struct Move moves[MAX_MOVES];
	return gen_legal_moves(moves, pos) == 0;
}

size_t
position_perft_inner(struct Board *pos, size_t depth)
{
	assert(depth >= 1);
	struct Move *moves = malloc(sizeof(struct Move) * MAX_MOVES);
	size_t moves_count_by_move[MAX_MOVES] = { 0 };
	size_t total_moves_count = gen_legal_moves(moves, pos);
	size_t result = 0;
	if (depth == 1) {
		result = total_moves_count;
	} else {
		for (size_t i = 0; i < total_moves_count; i++) {
			moves_count_by_move[i] = position_perft_inner(pos, depth - 1);
			result += moves_count_by_move[i];
		}
	}
	free(moves);
	return result;
}
