/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "game.h"
#include "chess/move.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

struct Game*
game_new(void)
{
	struct Game* game = xmalloc(sizeof(struct Game));
	game->tags = xmalloc(sizeof(struct GameTag));
	game->tags->next = NULL;
	game->num_moves = 0;
	game->moves_capacity = 8;
	game->moves = xmalloc(sizeof(Move) * game->moves_capacity);
	return game;
}

void
game_free(struct Game* game)
{
	free(game->tags);
	// while (game->tags) {
	//	struct Gametag *next = game->tags->next;
	//	free(game->tags);
	//	game->tags = next;
	//}
	// TODO
	free(game->moves);
	free(game);
}

void
game_print(struct Game* game)
{
	struct GameTag* tags = game->tags;
	while (tags) {
		printf("[%s \"%s\"]\n", tags->name, tags->value);
		tags = tags->next;
	}
	size_t i;
	for (i = game->num_moves; i > 0; i--) {
		// TODO: print moves.
	}
}

int_fast8_t
game_push(struct Game* game, Move move)
{
	if (game->num_moves == game->moves_capacity) {
		Move* moves;
		game->moves_capacity <<= 1;
		moves = realloc(game->moves, sizeof(Move) * game->moves_capacity);
		if (!moves) {
			return -1;
		} else {
			game->moves = moves;
		}
	}
	(*game->moves)[game->num_moves++] = move;
	return 0;
}
