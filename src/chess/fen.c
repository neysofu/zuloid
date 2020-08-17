#include "chess/fen.h"
#include "chess/color.h"
#include "chess/coordinates.h"
#include "chess/move.h"
#include "chess/pieces.h"
#include "chess/position.h"
#include "utils.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char FEN_OF_INITIAL_POSITION[FEN_SIZE] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

char *
fen_write_position_pieces(char *fen, const struct Board *position)
{
	Rank rank = RANK_MAX;
	do {
		bool last_piece_type_was_none = false;
		for (File file = 0; file <= FILE_MAX; file++) {
			Square square = square_new(file, rank);
			struct Piece piece = position_piece_at_square(position, square);
			if (piece.type) {
				*fen++ = piece_to_char(piece);
				last_piece_type_was_none = false;
			} else if (last_piece_type_was_none) {
				*(fen - 1) += 1;
			} else {
				*fen++ = '1';
				last_piece_type_was_none = true;
			}
		}
		if (rank) {
			*fen++ = '/';
		}
	} while (rank--);
	return fen;
}

char *
fen_from_position(char *fen, const struct Board *position, char sep)
{
	assert(position);
	if (!fen && !(fen = malloc(FEN_SIZE))) {
		return NULL;
	}
	char *fen_copy = fen;
	fen = fen_write_position_pieces(fen, position);
	*fen++ = sep;
	*fen++ = color_to_char(position->side_to_move);
	*fen++ = sep;
	if (position->castling_rights & (CASTLING_RIGHT_KINGSIDE << COLOR_WHITE)) {
		*fen++ = 'K';
	}
	if (position->castling_rights & (CASTLING_RIGHT_QUEENSIDE << COLOR_WHITE)) {
		*fen++ = 'Q';
	}
	if (position->castling_rights & (CASTLING_RIGHT_KINGSIDE << COLOR_BLACK)) {
		*fen++ = 'k';
	}
	if (position->castling_rights & (CASTLING_RIGHT_QUEENSIDE << COLOR_BLACK)) {
		*fen++ = 'q';
	}
	if (*(fen - 1) == sep) {
		*fen++ = '-';
	}
	*fen++ = sep;
	if (position->en_passant_target == SQUARE_NONE) {
		*fen++ = '-';
	} else {
		*fen++ = file_to_char(square_file(position->en_passant_target));
		*fen++ = rank_to_char(square_rank(position->en_passant_target));
	}
	*fen++ = sep;
	snprintf(
	  fen, 13, "%zu%c%zu", position->reversible_moves_count, sep, position->moves_count);
	return fen_copy;
}

int
position_init_from_fen_fields(struct Board *pos, const char **fieldsptr)
{
	position_empty(pos);
	const char *token = fieldsptr[0];
	/* Ranks are marked by slashed, so we need fen++ to get past them. */
	for (Rank rank = RANK_MAX; rank >= 0; rank--) {
		for (File file = 0; *token && !isspace(*token) && file <= FILE_MAX; token++, file++) {
			if (isdigit(*token)) {
				file += *token - '1';
			} else {
				Square square = square_new(file, rank);
				position_set_piece_at_square(pos, square, char_to_piece(*token));
			}
		}
		if ('/' == *token) {
			token++;
		}
	}
	token = fieldsptr[1];
	switch (tolower(*token)) {
		case 'w':
			pos->side_to_move = COLOR_WHITE;
			break;
		case 'b':
			pos->side_to_move = COLOR_BLACK;
			break;
		default:
			return ERR_CODE_INVALID_FEN;
	}
	token = fieldsptr[2];
	pos->castling_rights = string_to_castling_rights(token);
	token = fieldsptr[3];
	if (strlen(token) >= 2) {
		pos->en_passant_target = square_from_str(token);
	}
	if ((token = fieldsptr[4])) {
		pos->reversible_moves_count = strtol(token, NULL, 10);
	}
	if ((token = fieldsptr[5])) {
		pos->moves_count = strtol(token, NULL, 10);
	}
	return ERR_CODE_NONE;
}

int
position_init_from_fen(struct Board *pos, const char *fen)
{
	assert(pos);
	assert(fen);
	const char *fieldsptr[6] = { NULL };
	fieldsptr[0] = fen;
	for (size_t i = 1; i < 6 && fieldsptr[i-1]; i++) {
		const char *token = strpbrk(fieldsptr[i-1], " _") + 1;
		if (token && *token) {
			fieldsptr[i] = token;
		}
	}
	return position_init_from_fen_fields(pos, fieldsptr);
}
