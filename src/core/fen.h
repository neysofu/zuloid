#pragma once

#include "board.h"

// See [1] for reference.
//
//  64 + | pieces
//   7 + | slashes
//   1 + | active color
//   4 + | castling rights
//   2 + | en-passant target square
//   2 + | half-moves clock
//   3 + | full-moves
//   5 = | spaces
// -----
//  88
//
// [1]: https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
#define FEN_MAX_LENGTH 88
#define FEN_STARTPOS "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

char *
board_to_fen(const struct Board *board, char *fen);

struct Board *
fen_to_board(char *fen, struct Board *board);

void
fen_specify_pieces(char *fen_fragment, struct Board *board);

void
fen_specify_active_color(char *fen_fragment, struct Board *board);

void
fen_specify_castling_rights(char *fen_fragment, struct Board *board);

void
fen_specify_en_passant_file(char *fen_fragment, struct Board *board);

void
fen_specify_half_moves(char *fen_fragment, struct Board *board);

void
fen_specify_full_moves(char *fen_fragment, struct Board *board);
