#include "chess/color.h"
#include "chess/coordinates.h"
#include "chess/fen.h"
#include "chess/pieces.h"
#include "chess/position.h"
#include <ctype.h>

struct Chess960SetupState
{
	struct Board *position;
	File available_files[FILES_COUNT];
	size_t available_files_count;
};

static void
set_pieces_at_home_rank(struct Board *position, File file, enum PieceType ptype)
{
	for (int color = 0; color < 2; color++) {
		struct Piece piece = { ptype, color };
		Square square = square_new(file, color_home_rank(color));
		position_set_piece_at_square(position, square, piece);
	}
}

static int
files_compare(const void *f1, const void *f2)
{
	return *(File *)f1 - *(File *)f2;
}

static void
init_file(struct Chess960SetupState *state, File i, enum PieceType ptype)
{
	set_pieces_at_home_rank(state->position, state->available_files[i], ptype);
	state->available_files[i] = state->available_files[--state->available_files_count];
}

void
position_init_960(struct Board *position)
{
	position_init_from_fen(position, "8/pppppppp/8/8/8/8/PPPPPPPP/8 w KQkq - 0 1");
	struct Chess960SetupState state = {
		.position = position,
		.available_files = { 0, 1, 2, 3, 4, 5, 6, 7 },
		.available_files_count = 8,
	};
	// `rand() % n` is biased, remember.
	init_file(&state, (rand() % 4) * 2 + 1, PIECE_TYPE_BISHOP);
	init_file(&state, (rand() % 4) * 2, PIECE_TYPE_BISHOP);
	init_file(&state, rand() % 6, PIECE_TYPE_QUEEN);
	init_file(&state, rand() % 5, PIECE_TYPE_KNIGHT);
	init_file(&state, rand() % 4, PIECE_TYPE_KNIGHT);
	qsort(state.available_files, 3, sizeof(File), files_compare);
	init_file(&state, 0, PIECE_TYPE_ROOK);
	init_file(&state, 1, PIECE_TYPE_KING);
	init_file(&state, 2, PIECE_TYPE_ROOK);
}
