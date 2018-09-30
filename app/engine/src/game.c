#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "chess/move.h"

struct Game *
game_new(void) {
	struct Game *game = malloc(sizeof(struct Game));
	if (!game) {
		goto drop_at_game;
	}
	game->tags = malloc(sizeof(struct GameTag));
	if (!game->tags) {
		goto drop_at_tags;
	}
	game->tags->next = NULL;
	game->num_moves = 0;
	game->moves_capacity = 8;
	game->moves = malloc(sizeof(Move) * game->moves_capacity);
	if (!game->moves) {
		goto drop_at_moves;
	}
	return game;
drop_at_moves:
	free(game->moves);
drop_at_tags:
	free(game->tags);
drop_at_game:
	free(game);
	return NULL;
}

void
game_drop(struct Game *game) {
	free(game->tags);
	while (game->tags) {
		struct Gametag *next = game->tags->next;
		free(game->tags);
		game->tags = next;
	}
	free(game->moves);
	free(game);
}

void
game_print(struct Game *game) {
	struct GameTag *tags = game->tags;
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
game_push(struct Game *game, Move move) {
	if (game->num_moves == game->moves_capacity) {
		Move *moves;
		game->moves_capacity <<= 1;
		moves = realloc(game->moves,
			            sizeof(Move) * game->moves_capacity);
		if (!moves) {
			return -1;
		} else {
			game->moves = moves;
		}
	}
	(*game->moves)[game->num_moves++] = move;
	return 0;
}
