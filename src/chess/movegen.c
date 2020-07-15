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
#include "globals.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define EMIT_MOVE(m, a, b)                                                                 \
	(m)->source = (a);                                                                     \
	(m)->target = (b);                                                                     \
	(m)->promotion = 0;                                                                    \
	(m)++;

/* Generates all pseudolegal moves by pawns located on 'sources' to 'targets' target
 * squares. 'all' gives information regarding piece occupancy for determining pawn pushes
 * and captures. Finally, 'side_to_move' determines in which direction pawn moves happen. */
size_t
gen_pawn_moves(struct Move moves[],
               Bitboard sources,
               Bitboard targets,
               Bitboard all,
               enum Color side_to_move)
{
	struct Move *ptr = moves;
	Bitboard single_pushes =
	  (side_to_move == COLOR_WHITE ? sources << 1 : sources >> 1) & ~all;
	Bitboard double_pushes =
	  (side_to_move == COLOR_WHITE ? single_pushes << 1 : single_pushes >> 1) & ~all &
	  targets & rank_to_bb(color_double_push_rank(side_to_move));
	single_pushes &= targets;
	Bitboard captures_a = sources & ~file_to_bb(0);
	Bitboard captures_h = sources & ~file_to_bb(7);
	Square square;
	switch (side_to_move) {
		case COLOR_WHITE:
			captures_a >>= 7;
			captures_h <<= 7;
			captures_a &= targets & all;
			captures_h &= targets & all;
			while (single_pushes) {
				POP_LSB(square, single_pushes);
				EMIT_MOVE(moves, square - 1, square);
			}
			while (double_pushes) {
				POP_LSB(square, double_pushes);
				EMIT_MOVE(moves, square - 2, square);
			}
			while (captures_a) {
				POP_LSB(square, captures_a);
				EMIT_MOVE(moves, square + 7, square);
			}
			while (captures_h) {
				POP_LSB(square, captures_h);
				EMIT_MOVE(moves, square - 7, square);
			}
			break;
		case COLOR_BLACK:
			captures_a <<= 7;
			captures_h >>= 7;
			captures_a &= targets & all;
			captures_h &= targets & all;
			while (single_pushes) {
				POP_LSB(square, single_pushes);
				EMIT_MOVE(moves, square + 1, square);
			}
			while (double_pushes) {
				POP_LSB(square, double_pushes);
				EMIT_MOVE(moves, square + 2, square);
			}
			while (captures_a) {
				POP_LSB(square, captures_a);
				EMIT_MOVE(moves, square - 9, square);
			}
			while (captures_h) {
				POP_LSB(square, captures_h);
				EMIT_MOVE(moves, square + 9, square);
			}
			break;
		default:
			assert(0);
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
		Bitboard targets = BB_ATTACKS_BY_KNIGHT[source] & mask;
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
		Bitboard targets = BB_ATTACKS_BY_KING[source] & mask;
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
	// Bitboard mask, maskk, foo;
	///* TODO */
	// if (pos->castling_rights & (CASTLING_RIGHT_KINGSIDE << pos->side_to_move)) {
	//	mask = position_castle_mask(pos, CASTLING_RIGHT_KINGSIDE);
	//	position_flip_side_to_move(pos);
	//	maskk = mask;
	//	POP_LSB(foo, maskk);
	//	POP_MSB(foo, maskk);
	//	if (!(position_occupancy(pos) & maskk)) {
	//		// struct Move dummy[255];
	//		// if (!gen_attacks_against(dummy, pos, mask)) {
	//		EMIT_MOVE(moves,
	//		          bb_to_square(pos->bb[PIECE_TYPE_KING] & pos->bb[pos->side_to_move]),
	//		          square_new(6, color_home_rank(pos->side_to_move)));
	//		//}
	//	}
	//	position_flip_side_to_move(pos);
	//}
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
                         enum Color attacker)
{
	struct Move *ptr = moves;
	Bitboard pieces = pos->bb[attacker];
	moves += gen_pawn_moves(
	  moves, pieces & pos->bb[PIECE_TYPE_PAWN], victims, position_occupancy(pos), attacker);
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
gen_pseudolegal_moves(struct Move moves[], struct Board *pos)
{
	return gen_attacks_against_from(
	  moves, pos, ~pos->bb[pos->side_to_move], pos->side_to_move);
}

bool
position_is_illegal(struct Board *pos)
{
	struct Move moves[MAX_MOVES];
	return gen_attacks_against_from(moves,
	                                pos,
	                                pos->bb[pos->side_to_move] & pos->bb[PIECE_TYPE_KING],
	                                color_other(pos->side_to_move));
}

size_t
gen_legal_moves(struct Move moves[], struct Board *pos)
{
	int i = 0;
	int j = gen_pseudolegal_moves(moves, pos) - 1;
	while (i <= j) {
		struct Move move = moves[i];
		position_do_move(pos, &move);
		char buf[6];
		move_to_string(move, buf);
		if (position_is_illegal(pos)) {
			moves[i] = moves[j--];
		}
		position_undo_move(pos, &move);
		i++;
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

size_t
position_perft(FILE *stream, struct Board *pos, size_t depth)
{
	if (depth == 0) {
		fputs("1\n", stream);
		return 1;
	} else if (depth > MAX_DEPTH) {
		fprintf(stream, "<depth limit exceeded>\n");
		return 0;
	}
	struct Move *moves = malloc(sizeof(struct Move) * MAX_MOVES);
	size_t moves_count_by_move[MAX_MOVES];
	for (size_t i = 0; i < MAX_MOVES; moves_count_by_move[i++] = 1)
		;
	size_t root_moves_count = gen_legal_moves(moves, pos);
	size_t result = 0;
	if (depth == 1) {
		result = root_moves_count;
	} else {
		for (size_t i = 0; i < root_moves_count; i++) {
			position_do_move_and_flip(pos, &moves[i]);
			moves_count_by_move[i] = position_perft_inner(pos, depth - 1);
			result += moves_count_by_move[i];
			position_undo_move_and_flip(pos, &moves[i]);
		}
	}
	// Print user-friendly report for debugging.
	char mv_as_str[MOVE_STRING_MAX_LENGTH] = { '\0' };
	fprintf(stream, "%zu\n", result);
	for (size_t i = 0; i < root_moves_count; i++) {
		move_to_string(moves[i], mv_as_str);
		fprintf(stream, "%s\t%zu\n", mv_as_str, moves_count_by_move[i]);
	}
	free(moves);
	return result;
}
