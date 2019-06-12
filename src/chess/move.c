/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chess/move.h"
#include "chess/coordinates.h"
#include "chess/pieces.h"
#include "chess/position.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t
move_to_string(struct Move mv, char *buf)
{
	assert(buf);
	size_t i = 0;
	buf[i++] = file_to_char(square_file(mv.source));
	buf[i++] = rank_to_char(square_rank(mv.source));
	buf[i++] = file_to_char(square_file(mv.target));
	buf[i++] = rank_to_char(square_rank(mv.target));
	if (mv.promotion) {
		buf[i++] = piece_to_char((struct Piece){ .type = mv.promotion });
	}
	return i;
}

size_t
string_to_move(const char *str, struct Move *mv)
{
	assert(str);
	assert(mv);
	if (strlen(str) < 4) {
		return 0;
	}
	mv->source = square_new(char_to_file(str[0]), char_to_rank(str[1]));
	mv->target = square_new(char_to_file(str[2]), char_to_rank(str[3]));
	mv->promotion = char_to_piece(str[4]).type;
	return 4;
}

void
position_do_move(struct Position *pos, struct Move *mv)
{
	mv->capture = position_piece_at_square(pos, mv->target).type;
	position_set_piece_at_square(
	  pos, mv->target, position_piece_at_square(pos, mv->source));
	position_set_piece_at_square(pos, mv->source, PIECE_NONE);
	pos->side_to_move = color_other(pos->side_to_move);
}

void
position_undo_move(struct Position *pos, const struct Move *mv)
{
	position_set_piece_at_square(
	  pos, mv->source, position_piece_at_square(pos, mv->target));
	position_set_piece_at_square(
	  pos, mv->target, (struct Piece){ .type = mv->capture, .color = pos->side_to_move });
	pos->side_to_move = color_other(pos->side_to_move);
}

int
move_file_diff(struct Move *mv)
{
	return square_file(mv->target) - square_file(mv->source);
}

int
move_rank_diff(struct Move *mv)
{
	return square_rank(mv->target) - square_rank(mv->source);
}

Bitboard
move_ray(struct Move *mv)
{
	Bitboard ret = mv->target & (mv->source - 1);
	// switch (movement_between_two_squares(mv->source, mv->target)) {}
}

// bool
// position_check_castling_legality(struct Position *pos, struct Move *mv)
//{
//	int file_diff = move_file_diff(mv);
//	if (file_diff > 0) {
//		return pos->castling_rights[pos->side_to_move] & CASTLING_RIGHT_KINGSIDE;
//	} else {
//		return pos->castling_rights[pos->side_to_move] & CASTLING_RIGHT_QUEENSIDE;
//	}
//	/* TODO: Check for threats and checks. */
//}
//
// void
// position_do_move(struct Position *pos, struct Move *mv)
//{
//	assert(position_check_pseudolegality(pos, mv));
//	if (move_is_capture(move, position) || piece == PIECE_PAWN) {
//		position->reversible_moves_count++;
//	} else {
//		position->reversible_moves_count = 0;
//	}
//	bool is_king_capture = position->bb_pieces[PIECE_KING] &
// bb_coord(move_target(move)); 	if (is_king_capture) { 		return (struct Result){
// .winner = active_color, 			.termination = TERMINATION_CHECKMATE,
//		};
//	}
//	if (false) {
//		// position->bb_occupancy &= position->target;
//		// position->bb_pieces[PIECE_PAWN] &= position->target;
//	} else if (piece == PIECE_PAWN) {
//		// position->bb_pieces[position->promotion] |= bb_coord(position->target);
//	} else if (false) { // TODO
//		                //	File rook_source_file;
//		                //	File rook_target_file;
//		                //	if (coord_file(position->source) == 2) {
//		                //		rook_source_file = 0;
//		                //		rook_target_file = 3;
//		                //	} else {
//		                //		rook_source_file = 7;
//		                //		rook_target_file = 5;
//		                //	}
//		                //	Coord rook = coord_new(rook_source_file,
//		                // move_source(move).rank); 	*position_square(position, rook)
//		                // = SQUARE_NONE; 	rook.file = rook_target_file;
//		                //	position_square(position->position, rook)->piece =
//		                // PIECE_ROOK; 	position_square(position, rook)->color =
//		                // position->active_color;
//	}
//	uint64_t source_mask = ~square_to_bb(mv->source);
//	uint64_t target_mask = square_to_bb(mv->target);
//	position->bb[piece.type] &= source_mask;
//	position->bb[piece.type] &= target_mask;
//	position->bb_occupancy &= source_mask;
//	position->bb_occupancy |= target_mask;
//	position->bb_colors = (position->bb_colors & ~target_mask) |
//	                      ((uint64_t)(active_color) << (uint64_t)(move_target(move)));
//	position_toggle_active_color(position);
//	if (pos->reversible_moves_count >= 75) {
//		return (struct Result){
//			.winner = COLOR_NONE,
//			.termination = TERMINATION_NO_CAPTURE,
//		};
//	}
//	return RESULT_NONE;
//}
//
// struct Result
// position_push_legal_move(struct Position *pos, struct Move mv, enum Piece pc)
//{
//	return position_push_pseudolegal_move(position, move, piece); // TODO
//}
//

bool
position_check_pawn_pseudolegality(struct Position *pos, struct Move *mv)
{
	assert(pos);
	bool is_on_home_rank = square_rank(mv->source) == color_pawn_rank(pos->side_to_move);
	int file_diff = move_file_diff(mv);
	int rank_diff = move_rank_diff(mv);
	if (file_diff == 0) {
		return (mv->capture != PIECE_TYPE_NONE) &&
		       (abs(rank_diff) <= 1 + is_on_home_rank) &&
		       (pos->side_to_move ? rank_diff < 0 : rank_diff > 0);
	} else if (abs(file_diff) == 1 && abs(rank_diff) == 1) {
		/* TODO: en passant */
		return (mv->capture == PIECE_TYPE_NONE) && (abs(rank_diff) == 1);
	} else {
		return false;
	}
}

bool
position_check_knights_pseudolegality(struct Position *pos, struct Move *mv)
{}

bool
position_check_rook_pseudolegality(struct Position *pos, struct Move *mv)
{
	if (mv->source == mv->target) {
		return false;
	} else {
		// return (move_file_diff(mv) == 0 || move_rank_diff(mv) == 0) &&
		//       !(ray & pos->bb[COLOR_WHITE] & pos->bb[COLOR_BLACK]);
	}
}

bool
position_check_pseudolegality(struct Position *pos, struct Move *mv)
{
	Bitboard color_bb = pos->bb[pos->side_to_move];
	if (!(color_bb & square_to_bb(mv->source))) {
		return false;
	}
	enum PieceType pctype = position_piece_at_square(pos, mv->source).type;
	switch (pctype) {
		case PIECE_TYPE_PAWN:
			return position_check_pawn_pseudolegality(pos, mv);
		case PIECE_TYPE_KNIGHT:
			return position_check_knights_pseudolegality(pos, mv);
		case PIECE_TYPE_ROOK:
			return position_check_rook_pseudolegality(pos, mv);
		case PIECE_TYPE_KING:
			return false;
			//(square_king_threats(mv->source) & square_to_bb(mv->target)) ||
			//       (position_check_castling_legality(pos, mv));
			//       (position_check_castling_legality(pos, mv));
		default:
			return false; // TODO !(bb_ray(mv) & pos->bb_occupancy);
	}
}
