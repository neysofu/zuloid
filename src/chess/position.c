#include "chess/position.h"
#include "chess/bb.h"
#include "chess/color.h"
#include "chess/coordinates.h"
#include "chess/fen.h"
#include "chess/move.h"
#include "chess/pieces.h"
#include "utils.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
char_to_castling_right(char c)
{
	switch (c) {
		case 'K':
			return CASTLING_RIGHT_KINGSIDE << COLOR_WHITE;
		case 'Q':
			return CASTLING_RIGHT_QUEENSIDE << COLOR_WHITE;
		case 'k':
			return CASTLING_RIGHT_KINGSIDE << COLOR_BLACK;
		case 'q':
			return CASTLING_RIGHT_QUEENSIDE << COLOR_BLACK;
		default:
			return CASTLING_RIGHT_NONE;
	}
}

int
string_to_castling_rights(const char *str)
{
	assert(str);
	int crights = CASTLING_RIGHT_NONE;
	for (; *str; str++) {
		crights |= char_to_castling_right(*str);
	}
	return crights;
}

enum Color
position_flip_side_to_move(struct Board *pos)
{
	pos->side_to_move = color_other(pos->side_to_move);
	return pos->side_to_move;
}

void
position_zobrist(const struct Board *position) {

}

void
position_set_piece_at_square(struct Board *position, Square square, struct Piece piece)
{
	Bitboard bb = square_to_bb(square);
	for (size_t i = 0; i < POSITION_BB_COUNT; i++) {
		position->bb[i] &= ~bb;
	}
	switch (piece.type) {
		case PIECE_TYPE_QUEEN:
			position->bb[PIECE_TYPE_BISHOP] |= bb;
			position->bb[PIECE_TYPE_ROOK] |= bb;
			break;
		case PIECE_TYPE_NONE:
			return;
		default:
			position->bb[piece.type] |= bb;
			break;
	}
	position->bb[piece.color] |= bb;
}

struct Piece
position_piece_at_square(const struct Board *position, Square square)
{
	assert(position);
	Bitboard bb = square_to_bb(square);
	struct Piece piece = PIECE_NONE;
	enum PieceType i = PIECE_TYPE_FIRST_PRIMITIVE;
	do {
		if (position->bb[i] & bb) {
			piece.type += i;
		}
	} while (i++ < PIECE_TYPE_LAST_PRIMITIVE);
	if (position->bb[COLOR_BLACK] & bb) {
		piece.color = COLOR_BLACK;
	}
	return piece;
}

Bitboard
position_occupancy(struct Board *pos)
{
	return pos->bb[COLOR_WHITE] | pos->bb[COLOR_BLACK];
}

Bitboard
position_castle_mask(const struct Board *pos, int castling_right)
{
	Bitboard rank = rank_to_bb(color_home_rank(pos->side_to_move));
	Bitboard source = pos->bb[PIECE_TYPE_KING] & rank;
	Bitboard target;
	switch (castling_right) {
		case CASTLING_RIGHT_KINGSIDE:
			target = square_to_bb(square_new(6, color_home_rank(pos->side_to_move)));
			break;
		case CASTLING_RIGHT_QUEENSIDE:
			target = square_to_bb(square_new(2, color_home_rank(pos->side_to_move)));
			break;
		default:
			target = 0;
			assert(false);
	}
	return (((target - 1) ^ (source - 1)) | (source | target)) & rank;
}

void
position_empty(struct Board *position)
{
	assert(position);
	*position = (struct Board){
		.side_to_move = COLOR_WHITE,
		.en_passant_target = SQUARE_NONE,
		.castling_rights = CASTLING_RIGHTS_ALL,
		.reversible_moves_count = 0,
		.moves_count = 1,
	};
}

void
position_pprint(struct Board *position, FILE *stream)
{
	fprintf(stream, "     A B C D E F G H\n");
	fprintf(stream, "   +-----------------+\n");
	for (Rank rank = RANK_MAX; rank >= 0; rank--) {
		fprintf(stream, " %c | ", rank_to_char(rank));
		for (File file = 0; file <= FILE_MAX; file++) {
			struct Piece piece = position_piece_at_square(position, square_new(file, rank));
			fprintf(stream, "%c ", piece_to_char(piece));
		}
		fprintf(stream, "| %c\n", rank_to_char(rank));
	}
	fprintf(stream, "   +-----------------+\n");
	fprintf(stream, "     A B C D E F G H\n");
	char *fen = fen_from_position(NULL, position, ' ');
	fprintf(stream, "FEN: %s\n", fen);
	free(fen);
}

const struct Board POSITION_INIT = {
  .bb =
    {
      [COLOR_WHITE] = 0x0303030303030303,
      [COLOR_BLACK] = 0xc0c0c0c0c0c0c0c0,
      [PIECE_TYPE_PAWN] = 0x4242424242424242,
      [PIECE_TYPE_KNIGHT] = 0x0081000000008100,
      [PIECE_TYPE_BISHOP] = 0x0000810081810000,
      [PIECE_TYPE_ROOK] = 0x8100000081000081,
      [PIECE_TYPE_KING] = 0x0000008100000000,
    },
  .side_to_move = COLOR_WHITE,
  .en_passant_target = SQUARE_NONE,
  .castling_rights = CASTLING_RIGHTS_ALL,
  .reversible_moves_count = 0,
  .moves_count = 1,
};
