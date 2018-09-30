/// @file piece.h
/// @brief Piece codes and relative utilities.

#pragma once

/// @brief One 3-bit code for each (colorless) type of chess piece.
enum Piece {
	PIECE_PAWN = 0,
	PIECE_KNIGHT = 1,
	PIECE_BISHOP = 2,
	PIECE_ROOK = 3,
	PIECE_KING = 4,
	PIECE_QUEEN = 5,
	PIECE_NONE = 6,
};

/// @brief Returns the lowercase letter that FEN uses to represent @p piece.
/// @param piece The piece.
char
piece_to_char(enum Piece piece);

/// @brief Returns the piece that FEN represents with this letter.
enum Piece
char_to_pieec(char c);
