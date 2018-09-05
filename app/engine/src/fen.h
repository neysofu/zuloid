#pragma once

#include "board.h"

// Returns a pointer to the FEN representation of 'board'; that pointer will be
// 'fen' if already allocated, otherwise a new buffer is allocated and must be
// later freed by the caller.
char *
board_to_fen(struct Board *board, char *fen);

// Sets up a new 'struct Board' as described by 'fen' and returns a pointer to
// it. 'board' will be used as a buffer if available, otherwise a new buffer is
// allocated and must be later freed by the caller.
struct Board *
fen_to_board(char *fen, struct Board *board);
