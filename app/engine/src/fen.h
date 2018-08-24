#pragma once

#include "board.h"

// See [0] for reference.
//
//  64 + | pieces
//   7 + | slashes
//   1 + | active color
//   4 + | castling rights
//   2 + | en-passant target square
//   2 + | half-moves clock
//   4 + | full-moves
//   5 = | spaces
// -----
//  89
//
// [0]: https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
#define FEN_MAX_LENGTH 89
#define FEN_STARTPOS "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

// Return a pointer to the FEN representation of 'board'. If 'fen' is set to
// NULL, then a new buffer is allocated and must be later freed by the caller;
// otherwise, 'fen' is assumed to be a pointer to 'FEN_MAX_LENGTH' bytes or more
// and is overwritten.
char *
board_to_fen(struct Board *board, char *fen);

void
fen_to_board(char *fen, struct Board *board);
