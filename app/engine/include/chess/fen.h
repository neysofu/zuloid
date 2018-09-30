/// @file fen.h
/// @brief Function prototypes for FEN utilities.
//
/// FEN strings
/// (Forsyth–Edwards Notation, see the wikipedia page
/// for details) are not used internally by Z64C, but
/// they are available as means of input and output for chess positions.
//
/// @see https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation

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
#define FEN_SIZE 128

/// Writes the FEN string to @buffer from the chess position @board.
///
/// \pre @buffer points to a free memory location of at least 90 bytes.
///
/// @param board the source chess position.
/// @param buffer a pointer to the target memory location at which the FEN
/// string will be written.
void
board_to_fen(struct Board *board, char *buffer);

/// Sets up a chess position @p buffer as described by the FEN string located at
/// @p fen.
///
/// @param fen A pointer to the source FEN string.
/// @param buffer A pointer to the target memory location.
///
/// @return A non-zero error code on failure, zero on success. The possible
/// error codes are:
/// - \b 1. Missing field(s).
/// - \b 2. Impossible chess position.
/// - \b {From 10 to 15}. Syntax error in the \a nth field.
/// - \b -1. Catch-all error code for any other issue.
///
/// @pre FEN fields in @c fen are whitespace-separated. The last two fields are
/// optional and either one or both of them may be absent.
int8_t
fen_to_board(char *fen, struct Board *buffer);
