#pragma once

#include "board.h"

#define FEN_STARTPOS "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

char *
board_to_fen(const struct Board *board);

void
fen_specify_pieces(char *fen_fragment, struct Board *board);

void
fen_specify_active_player(char *fen_fragment, struct Board *board);

void
fen_specify_castling_rights(char *fen_fragment, struct Board *board);

void
fen_specify_en_passant_target(char *fen_fragment, struct Board *board);

void
fen_specify_half_moves(char *fen_fragment, struct Board *board);

void
fen_specify_full_moves(char *fen_fragment, struct Board *board);
