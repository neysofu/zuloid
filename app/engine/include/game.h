/// @file game.h
/// @brief Operations that allow to store game metainformation and PGN
/// utilities.
///
/// FEN strings
/// (Forsyth–Edwards Notation, see the wikipedia page
/// for details) are not used internally by Z64C, but
/// they are available as means of input and output for chess positions.
//
/// @see https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation

#pragma once

#include <stdlib.h>
#include "chess/board.h"
#include "chess/move.h"

struct GameTag {
	char *name;
	char *value;
	struct GameTag *next;
};

// @struct Game
// @brief A container for game metainformation.
struct Game {
	struct GameTag *tags;
	size_t num_moves;
	size_t moves_capacity;
	Move (*moves)[];
};

// @brief Creates a new empty game.
// @return A pointer to the newly-allocated object.
struct Game *
game_new(void);

// @brief Frees the game.
void
game_drop(struct Game *game);

/// @brief Adds a name/value tag to this game.
///
/// @return A non-zero error code on failure, zero on success. The only possible
/// error code is \b -1 for allocation failure.
int_fast8_t
game_add_tag(struct Game *game, char *tag_name, char *tag_value);

// Converts this game to a PGN string and writes it to a buffer.
void
game_to_pgn(struct Game *game, char *buffer, size_t buffer_size);

// Updates this game with a move.
int_fast8_t
game_push(struct Game *game, Move move);
