#include <string.h>
#include "chessboard.h"

void
cb_specify_pieces(struct Chessboard *cb, char *fen_fragment) {
	Rank rank = CHESSBOARD_SIDE_LENGHT;
	File file = 0;
	char *token;
	while ((token = strtok(fen_fragment, "/")), rank-->0) {
		file += atoi(token + file);
		while (file < CHESSBOARD_SIDE_LENGHT) {
			if (token[file] > 'a') {
				cb->squares[rank][file].color = COLOR_BLACK;
				token[file] += ('A' - 'a');
			} else {
				cb->squares[rank][file].color = COLOR_WHITE;
			}
			cb->squares[rank][file].piece = token[file];
		}
	}
}

void
cb_specify_active_player(struct Chessboard *cb, const char *fen_fragment) {
	switch (fen_fragment[0]) {
		case 'w':
			cb->active_color = COLOR_WHITE;
			break;
		case 'b':
			cb->active_color = COLOR_BLACK;
			break;
		default:
			return;
	}
}

void
cb_specify_castling_rights(struct Chessboard *cb, const char *fen_fragment) {
	uint8_t i = 0;
	while (fen_fragment[i] != '\0') {
		switch (fen_fragment[i]) {
			case 'K':
				cb->castling_rights.white_kingside = true;
				break;
			case 'Q':
				cb->castling_rights.white_queenside = true;
				break;
			case 'k':
				cb->castling_rights.black_kingside = true;
				break;
			case 'q':
				cb->castling_rights.black_queenside = true;
				break;
			case '-':
				cb->castling_rights.white_kingside = true;
				cb->castling_rights.white_queenside = true;
				cb->castling_rights.black_kingside = true;
				cb->castling_rights.black_queenside = true;
				break;
			default:
				return;
		}
		i++;
	}
}

void
cb_specify_en_passant_target(struct Chessboard *cb, const char *fen_fragment) {
	if (fen_fragment[0] == '-') {
		*(cb->en_passant_target) = IMPOSSIBLE;
	}
	*(cb->en_passant_target) = coord_from_algebraic_notation(fen_fragment);
}

void
cb_specify_half_moves(struct Chessboard *cb, const char *fen_fragment) {
	cb->half_moves = atoi(fen_fragment);
}

struct Chessboard *
cb_from_fen(char fen[]) {
	struct Chessboard *cb;
	char *token;
	if (!(token = strtok(fen, " "))) { return NULL; }
	cb_specify_pieces(cb, token);
	if (!(token = strtok(fen, " "))) { return NULL; }
	cb_specify_active_player(cb, token);
	if (!(token = strtok(fen, " "))) { return NULL; }
	cb_specify_castling_rights(cb, token);
	if (!(token = strtok(fen, " "))) { return NULL; }
	cb_specify_en_passant_target(cb, token);
	if (!(token = strtok(fen, " "))) { return NULL; }
	cb_specify_half_moves(cb, token);
	return cb;
}
