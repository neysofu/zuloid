#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sysexits.h>
#include "api.h"
#include "chess/board.h"
#include "chess/fen.h"
#include "chess/legality.h"
#include "chess/move.h"
#include "clock.h"
#include "cmd.h"
#include "driver.h"
#include "game.h"
#include "settings.h"
#include "xxHash.h"

#define MAX_PERFT_DEPTH 32

int8_t
driver_call_foo(struct Driver *driver, struct Cmd *cmd) {
	assert(driver);
	assert(cmd);
	printf("bar\n");
	return EX_OK;
}

int8_t
driver_call_load(struct Driver *driver, struct Cmd *cmd) {
	assert(driver);
	assert(cmd);
	if (driver->mode != MODE_IDLE) {
		fprintf(stderr,
				"error \"Z64C can only load sessions while in idle mode.\"\n");
		return EX_TEMPFAIL;
	}
	char *session_path = cmd_arg_ith(cmd, 1);
	if (!session_path) {
		return EX_USAGE;
	}
	// TODO
	return EX_OK;
}

int8_t
driver_call_move(struct Driver *driver, struct Cmd *cmd) {
	assert(driver);
	assert(cmd);
	if (driver->mode != MODE_IDLE) {
		printf("error \"Z64C must be in idle mode before making "
			   "changes to the chess position.\"\n");
		return EX_TEMPFAIL;
		// TODO.
	}
	char *move_as_str = cmd_arg_ith(cmd, 1);
	Move move = str_to_move(move_as_str);
	if (move == MOVE_NONE) {
		fprintf(stderr, "error \"Moves must be in coordinate notation.\"\n");
		return EX_DATAERR;
		// TODO
	}
	enum Piece piece = board_piece_at(&driver->board, move_source(move));
	if (board_approves_pseudolegal_move(&driver->board, move, piece)) {
		board_push_pseudolegal_move(&driver->board, move, piece);
		game_push(driver->game, move);
	} else {
		printf("error \"%s is illegal.\"\n", move_as_str);
		return EX_DATAERR;
	}
	return EX_OK;
}

int8_t
driver_call_perft(struct Driver *driver, struct Cmd *cmd) {
	assert(driver);
	assert(cmd);
	size_t depth = atoi(cmd_arg_ith(cmd, 1));
	// Error detection.
	if (depth == 0 && strcmp(cmd_arg_ith(cmd, 1), "0") != 0) {
		fprintf(stderr,
				"error \"Usage: perft [depth].\"\n");
		return EX_OSERR;
	}
	if (depth > MAX_PERFT_DEPTH) {
		fprintf(stderr,
				"error \"Too deep.\"\n");
		return EX_USAGE;
	}
	struct TTable *ttables = malloc(sizeof(struct TTable *) * depth);
	struct Board *board_buffer = malloc(sizeof(struct Board));
	*board_buffer = BOARD_STARTPOS;
	Move legal_moves[BOARD_MAX_NUM_LEGAL_MOVES];
	size_t counter;
	board_legal_moves(&driver->board, &legal_moves);
	for (counter = 0; counter < depth; counter++) {
		ttable_insert(ttables, board_buffer);
	}
	free(ttables);
	return EX_OK;
}

int8_t
driver_call_print(struct Driver *driver, struct Cmd *cmd) {
	assert(driver);
	assert(cmd);
	char *arg_1 = cmd_arg_ith(cmd, 1);
	char *buffer;
	// https://asecuritysite.com/encryption/xxHash
	switch (XXH64(arg_1, strlen(arg_1), 0)) {
		case 0x6c5f7b2e4a79fd6d: // "name"
			printf("%s\n", DRIVER_NAME);
			break;
		case 0x28c069b1c9d0a6ee: // "version"
			printf("%s\n", DRIVER_VERSION);
			break;
		case 0x05b755a1a530b7ce: // "release-date"
			printf("%s\n", DRIVER_RELEASE_DATE);
			break;
		case 0x7c77840f61186045: // "author"
			printf("%s\n", DRIVER_AUTHOR);
			break;
		case 0x0544b05ecc4a68a5: // "url"
			printf("%s\n", DRIVER_URL);
			break;
		case 0xe0c8b500ae055972: // "position:result"
			break;
		case 0x8a7ecc4137c6f2b0: // "position"
			board_print(&driver->board);
			break;
		case 0x3890d21a6b1d01b5: // ""
			// TODO
			break;
		case 0x6ef4a701e3e2a582: // "position:castling"
			if (driver->board.game_state & GAME_STATE_CASTLING_RIGHT_WK) {
				putchar('K');
			}
			if (driver->board.game_state & GAME_STATE_CASTLING_RIGHT_BK) {
				putchar('k');
			}
			if (driver->board.game_state & GAME_STATE_CASTLING_RIGHT_WQ) {
				putchar('Q');
			}
			if (driver->board.game_state & GAME_STATE_CASTLING_RIGHT_BQ) {
				putchar('q');
			}
			putchar('\n');
			break;
		case 0x29e651ba535a6cff: // "position:turn"
			if (board_active_color(&driver->board) == COLOR_WHITE) {
				putchar('w');
			} else {
				putchar('b');
			}
			putchar('\n');
			break;
		case 0xe255add3561ac54e: // "position:en-passant"
			putchar(board_en_passant_is_available(&driver->board) ? '1' : '0');
			putchar('\n');
			break;
		case 0x3dde2c8e2f1ae60c: // "position:fen"
			buffer = malloc(FEN_SIZE);
			memset(buffer, '\0', FEN_SIZE);
			board_to_fen(&driver->board, buffer);
			printf("%s\n", buffer);
			free(buffer);
			break;
		case 0xbaff859407a7adf9: // "position:pgn"
			// TODO "pgn"
			break;
		case 0x31170f08e32538e2: // "position:settings"
			settings_print(&driver->settings);
		default:
			if (strncmp(arg_1, "setting:", 8) == 0) {
				printf("%s\n", settings_value(&driver->settings, arg_1 + 8));
			} else {
				printf("error \"'%s' is not a valid value.\"\n", arg_1);
				return EX_DATAERR;
			}
			break;
	}
	return EX_OK;
}

int8_t
driver_call_purge(struct Driver *driver, struct Cmd *cmd) {
	assert(driver);
	assert(cmd);
	ttable_clear_fragments(driver->ttable);
	return EX_OK;
}

int8_t
driver_call_quit(struct Driver *driver, struct Cmd *cmd) {
	assert(driver);
	assert(cmd);
	driver->mode = MODE_QUIT;
	return EX_OK;
}

int8_t
driver_call_save(struct Driver *driver, struct Cmd *cmd) {
	assert(driver);
	assert(cmd);
	// TODO
	return EX_OK;
}

int8_t
driver_call_set(struct Driver *driver, struct Cmd *cmd) {
	assert(driver);
	assert(cmd);
	if (driver->mode != MODE_IDLE) {
		fprintf(stderr,
				"error \"You can't change settings while "
				"Z64C is thinking.\"\n");
		return EX_TEMPFAIL;
	}
	char *cmd_arg_1 = cmd_arg_ith(cmd, 1);
	char *cmd_arg_2 = cmd_arg_ith(cmd, 2);
	if (!(cmd_arg_1 && cmd_arg_2)) {
		fprintf(stderr,
				"error \"Invalid syntax. Usage: 'set [name] [value]'.\"\n");
		return EX_USAGE;
	}
	int8_t exit_status = settings_set_value(&driver->settings, cmd_arg_1, cmd_arg_2);
	if (exit_status != EX_OK) {
		fprintf(stderr, "error TODO\n");
	}
	return exit_status;
}

int8_t
driver_call_setup(struct Driver *driver, struct Cmd *cmd) {
	assert(driver);
	assert(cmd);
	if (driver->mode != MODE_IDLE) {
		fprintf(stderr,
				"error \"You can't set up a new chess position while "
				"Z64C is thinking.\"\n");
		return EX_TEMPFAIL;
	}
	char *cmd_arg_1 = cmd_arg_ith(cmd, 1);
	char *move_as_str;
	if (!cmd_arg_1) {
		driver->board = BOARD_STARTPOS;
	} else if (strcmp(cmd_arg_1, "960") == 0) {
		board_setup_960(&driver->board);
	} else {
		fen_to_board(cmd_arg_ith(cmd, 2), &driver->board);
		// TODO: error checking.
	}
	size_t i = 2;
	while ((move_as_str = cmd_arg_ith(cmd, i))) {
		Move move = str_to_move(move_as_str);
		enum Piece piece = board_piece_at(&driver->board, move_source(move));
		if (!board_approves_legal_move(&driver->board,
									   move,
									   piece)) {
			fprintf(stdout, "error \"One or more of the given moves are "
				   "illegal (%s).\"\n",
				   move_as_str);
			return EX_DATAERR;
		}
		driver->result = board_push_legal_move(&driver->board, move, piece);
	}
	return EX_OK;
}

int8_t
driver_call_think(struct Driver *driver, struct Cmd *cmd) {
	assert(driver);
	assert(cmd);
	char *arg_1 = cmd_arg_ith(cmd, 1);
	if (!arg_1) {
		// TODO
		return EX_NOINPUT;
	}
	if (strcmp(arg_1, "start") == 0) {
		Move best_move = ttable_best_move(driver->ttable);
	} else if (strcmp(arg_1, "stop") == 0) {
		if (driver->mode != MODE_BUSY) {
			fprintf(stderr,
				    "error \"Z64C can't stop the search because it wasn't "
			    	"searching in the first place.\"\n");
			return EX_TEMPFAIL;
		}
		driver->mode = MODE_IDLE;
	}
	return EX_OK;
}
