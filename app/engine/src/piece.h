#pragma once

// One 3-bit code for each type of (colorless) chess piece.
enum Piece {
	PIECE_PAWN = 0,
	PIECE_KNIGHT = 0x1,
	PIECE_BISHOP = 0x2,
	PIECE_ROOK = 0x3,
	PIECE_KING = 0x4,
	PIECE_QUEEN = 0x5,
	PIECE_NONE = 0x6,
};

// Returns the lowercase letter that FEN uses to represent the piece 'piece'.
char
piece_to_char(enum Piece piece);

// Returns the piece that FEN represents with the lowercase letter 'c'.
enum Piece
char_to_pieec(char *c);
