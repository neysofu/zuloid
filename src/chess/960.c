#include "chess/bb.h"
#include "chess/color.h"
#include "chess/coordinates.h"
#include "chess/move.h"
#include "chess/pieces.h"
#include "chess/position.h"
#include "utils.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const struct Board POSITION_INIT_WITH_ONLY_PAWNS = {
  .bb =
    {
      [COLOR_WHITE] = 0x0202020202020202,
      [COLOR_BLACK] = 0x4040404040404040,
      [PIECE_TYPE_PAWN] = 0x4242424242424242,
      [PIECE_TYPE_KNIGHT] = 0,
      [PIECE_TYPE_BISHOP] = 0,
      [PIECE_TYPE_ROOK] = 0,
      [PIECE_TYPE_KING] = 0,
    },
  .side_to_move = COLOR_WHITE,
  .en_passant_target = SQUARE_NONE,
  .castling_rights = CASTLING_RIGHTS_ALL,
  .reversible_moves_count = 0,
  .moves_count = 1,
};

void
set_pieces_at_home_rank(struct Board *position, File file, enum PieceType ptype)
{
	char piece = piece_to_char((struct Piece){ .type = ptype, .color = COLOR_WHITE });
	position_set_piece_at_square(position,
	                             square_new(file, color_home_rank(COLOR_WHITE)),
	                             char_to_piece(toupper(piece)));
	position_set_piece_at_square(position,
	                             square_new(file, color_home_rank(COLOR_BLACK)),
	                             char_to_piece(tolower(piece)));
}

int
files_compare(const void *f1, const void *f2)
{
	return *(File *)f1 - *(File *)f2;
}

struct Chess960InitializationState
{
	File available_files[FILES_COUNT];
	size_t i;
};

void
position_960_init_file(struct Board *position,
                       struct Chess960InitializationState *state,
                       int i,
                       enum PieceType ptype)
{
	set_pieces_at_home_rank(position, state->available_files[i], ptype);
	state->available_files[i] = state->available_files[state->i--];
}

void
position_init_960(struct Board *position)
{
	*position = POSITION_INIT_WITH_ONLY_PAWNS;
	struct Chess960InitializationState state = {
		.available_files = { 0, 1, 2, 3, 4, 5, 6, 7 },
		.i = 7,
	};
	// `rand() % n` is biased, remember.
	position_960_init_file(position, &state, (rand() % 4) * 2 + 1, PIECE_TYPE_BISHOP);
	position_960_init_file(position, &state, (rand() % 4) * 2, PIECE_TYPE_BISHOP);
	position_960_init_file(position, &state, rand() % 6, PIECE_TYPE_QUEEN);
	position_960_init_file(position, &state, rand() % 5, PIECE_TYPE_KNIGHT);
	position_960_init_file(position, &state, rand() % 4, PIECE_TYPE_KNIGHT);
	qsort(state.available_files, 3, sizeof(File), files_compare);
	position_960_init_file(position, &state, 0, PIECE_TYPE_ROOK);
	position_960_init_file(position, &state, 1, PIECE_TYPE_KING);
	position_960_init_file(position, &state, 2, PIECE_TYPE_ROOK);
}
