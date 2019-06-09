/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Resources:
 *  - http://wbec-ridderkerk.nl/html/UCIProtocol.html
 *  - https://www.chessprogramming.org/UCI
 *  - https://www.seungjaelee.com/projects/uci/ */

#include "agent.h"
#include "cache/cache.h"
#include "chess/fen.h"
#include "chess/position.h"
#include "engine.h"
#include "globals.h"
#include "utils.h"
#include "xxHash/xxhash.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

void
engine_uci_call_go(struct Engine *engine, char *cmd)
{
	if (engine->mode == MODE_SEARCH) {
		printf("# [ERROR] The engine is already searching.\n");
		return;
	}
	char *token = NULL;
	while ((token = strtok_whitespace(NULL))) {
		printf("# [INFO] The next token is '%s'.\n", token);
		switch (XXH64(token, strlen(token), 0)) {
			case 0x2a8ef3657cf9a920: /* "wtime" */
				token = strtok_whitespace(NULL);
				engine->time_controls[COLOR_WHITE]->time_limit_in_seconds =
				  atoi(token) * 1000;
				break;
			case 0xd3f6a6885c7c93a0: /* "btime" */
				token = strtok_whitespace(NULL);
				engine->time_controls[COLOR_BLACK]->time_limit_in_seconds =
				  atoi(token) * 1000;
				break;
			case 0x71c2388517319e0c: /* "winc" */
				token = strtok_whitespace(NULL);
				engine->time_controls[COLOR_WHITE]->increment_in_seconds =
				  atoi(token) * 1000;
				break;
			case 0xad513987341315ae: /* "binc" */
				token = strtok_whitespace(NULL);
				engine->time_controls[COLOR_BLACK]->increment_in_seconds =
				  atoi(token) * 1000;
				break;
			case 0x18ebef875e97de86: /* "infinite" */
				time_control_delete(engine->time_controls[COLOR_WHITE]);
				time_control_delete(engine->time_controls[COLOR_BLACK]);
				break;
			case 0x0a6f394a3987568a: /* "ponder" */
				engine->ponder = true;
				break;
			case 0xf95e9c242b5d7a1a: /* "movestogo" */
				token = strtok_whitespace(NULL);
				engine->time_controls[COLOR_WHITE]->max_moves_count = atoi(token);
				engine->time_controls[COLOR_BLACK]->max_moves_count = atoi(token);
				break;
			case 0xe142606f37a3b175: /* "depth" */
			case 0x3323f763484dfbd4: /* "mate" */
				token = strtok_whitespace(NULL);
				engine->max_depth = atoi(token);
				break;
			case 0xd6accb2d47332ac7: /* "nodes" */
				token = strtok_whitespace(NULL);
				engine->max_nodes_count = atoi(token);
				break;
			case 0x653009b7ced43713: /* "movetime" */
				token = strtok_whitespace(NULL);
				engine->game_clocks[engine->position.side_to_move].time_left_in_seconds =
				  atoi(token) * 1000;
				break;
			default:
				ENGINE_DEBUGF(engine, "[WARN] Ignoring unknown token: '%s'\n", token);
		}
	}
	engine_start_search(engine);
}

void
engine_uci_call_openlichessanalysis(struct Engine *engine, char *cmd)
{
	char *command = malloc(64 + FEN_SIZE);
	char *fen = fen_from_position(NULL, &engine->position, '_');
#ifdef __APPLE__
	sprintf(command, "open https://lichess.org/analysis/standard/%s\n", fen);
	system(command);
#elif __linux__
	sprintf(command, "xdg-open https://lichess.org/analysis/standard/%s\n", fen);
	system(command);
#elif _WIN32
	sprintf(command, "start https://lichess.org/analysis/standard/%s\n", fen);
	system(command);
#else
	ENGINE_DEBUGF(engine,
	              "[ERROR] This platform is not supported. Please file a bug report.");
	engine->mode = MODE_EXIT;
#endif
	free(command);
	free(fen);
}

void
engine_uci_call_position(struct Engine *engine, char *cmd)
{
	char *token = strtok_whitespace(NULL);
	if (!token) {
		ENGINE_DEBUGF(engine, "[ERROR] 'startpos' | 'fen' | '960' token expected.\n");
		return;
	} else if (strcmp(token, "startpos") == 0) {
		engine->position = POSITION_INIT;
	} else if (strcmp(token, "fen") == 0) {
		char *fen_fields[6] = { NULL };
		for (size_t i = 0; i < 6; i++) {
			token = strtok_whitespace(NULL);
			if (!token) {
				return;
			}
			fen_fields[i] = token;
		}
		position_init_from_fen_fields(&engine->position, fen_fields);
	} else if (strcmp(token, "960") == 0) {
		/* The "960" command is a custom addition the standard. I figured it could be useful
		 * for training. */
		position_init_960(&engine->position);
	} else {
		ENGINE_DEBUGF(engine, "[ERROR] 'startpos' | 'fen' | '960' token expected.\n");
	}
	/* Now feed moves into the position. */
	while ((token = strtok_whitespace(NULL))) {
		struct Move mv;
		string_to_move(token, &mv);
		position_do_move(&engine->position, &mv);
	}
}

void
engine_uci_call_setoption(struct Engine *engine, char *cmd)
{
	if (engine->mode != MODE_IDLE) {
		ENGINE_DEBUGF(engine, "[ERROR] UCI options can only be set during idle state.\n");
		return;
	}
	uint64_t hash = 0;
	char *token = NULL;
	while ((token = strtok_whitespace(NULL))) {
		if (strcmp(token, "value") == 0) {
			return;
		}
		/* From the UCI protocol specification (April 2004):
		 * > The name of the option in  should not be case sensitive and can inludes spaces
		 * > like also the value. */
		for (char *ptr = token; *ptr; ptr++) {
			*ptr = tolower(*ptr);
		}
		/* XOR combine the hashes. Simple yet effective. */
		hash ^= XXH64(token, strlen(token), 0);
	}
	/* Option support is quite hairy and messy. I don't want to break pre-existing scripts
	 * and configs originally written for other engines.
	 *
	 * Please see:
	 *  - https://komodochess.com/Komodo-11-README.html
	 *  - http://www.rybkachess.com/index.php?auswahl=Engine+parameters
	 *
	 * No worries in case the links above die, just search for a list of UCI settings for
	 * popular chess engines. I don't commit to 100% feature parity with any engine; I just
	 * try and use my better judgement. */
	switch (hash) {
		case 0xd8cdd8e8314c4147: /* "hash" */
			break;
		case 0xc057a61296095806: /* "nalimovpath" */
			break;
		case 0xcfaf77aca1f8feaa: /* "nalimovcache" */
			break;
		case 0x0a6f394a3987568a: /* "ponder" */
			engine->ponder = true;
			break;
		case 0x487e0e93e2c2bb18: /* "ownbook" */
			break;
		case 0xc8922f8e470ffaee: /* "uci_showcurrline" */
			break;
		case 0xdf718624bc46e2ce: /* "uci_showrefutations" */
			break;
		case 0xf6253b92eb36d560: /* "uci_limitstrength" */
			break;
		/* TODO: Many, many more. */
		default:
			ENGINE_DEBUGF(engine, "[ERROR] No such option.\n");
	}
}

void
engine_uci(struct Engine *engine, char *cmd)
{
	assert(engine);
	assert(cmd);
	char *token = strtok_whitespace(cmd);
	if (!token) {
		/* Ignore empty lines. */
		return;
	}
	switch (XXH64(token, strlen(token), 0)) {
		case 0x01fd51a2a6f9cc2f: /* "debug" */
			/* This command feels quite useless (in fact, Stockfish doesn't even recognize
			 * it). Nevertheless, we shall offer the option to send additional evaluation
			 * details with it. It does *not* control debugging information, which instead
			 * gets compiled out with the NDEBUG macro. */
			token = strtok_whitespace(NULL);
			if (!token) {
				ENGINE_DEBUGF(engine, "[ERROR] 'on' | 'off' token expected.\n");
			} else if (strcmp(token, "on") == 0) {
				engine->verbose = true;
			} else if (strcmp(token, "off") == 0) {
				engine->verbose = false;
			}
			break;
		case 0xd682e29388e0f4a3: /* "go" */
			engine_uci_call_go(engine, cmd);
			break;
		case 0x33e37def10e5d195: /* "isready" */
			printf("readyok\n");
			break;
		case 0x8a7ecc4137c6f2b0: /* "position" */
			engine_uci_call_position(engine, cmd);
			break;
		case 0x707db5f765aed6d8: /* "quit" */
			engine->mode = MODE_EXIT;
			break;
		case 0x26cc87cdbb3247ba: /* "setoption" */
			engine_uci_call_setoption(engine, cmd);
			break;
		case 0xd8d95334d91f61fc: /* "stop" */
			engine_stop_search(engine);
			break;
		case 0xf80028c1113b2c9c: /* "uci" */
			printf("id name Z64C/%s %s\n"
			       "id author Filippo Costa\n"
			       "option name Clear Hash type button\n"
			       /* TODO: also implement Komodo's Drawscore option. */
			       "option name Contempt type spin default 20 min -100 max 100\n"
			       "option name Hash type spin default 64 min 0 max 131072\n"
			       "option name Minimum Thinking Time type spin default 20 min 0 max 5000\n"
			       "option name nodestime type spin default 0 min 0 max 10000\n"
			       "option name Ponder type check default true\n"
			       "option name Skill Level type spin default 20 min 0 max 20\n"
			       /* See http://www.talkchess.com/forum3/viewtopic.php?start=0&t=42308 */
			       "option name Slow Mover type spin default 84 min 10 max 1000\n"
			       "option name Threads type spin default 1 min 1 max 512\n"
			       "option name Move Overhead type spin default 30 min 0 max 60000\n"
			       "uciok\n",
			       Z64C_BACKEND_NAME,
			       Z64C_VERSION);
			break;
		case 0xdfd89a3bb7b15ce5: /* "ucinewgame" */
			cache_clear(engine->cache);
			break;
		/* The following are non-standard commands implemented in Stockfish. */
		case 0x5000d8f2907d14e4: /* "d" */
			position_print(&engine->position);
			break;
		case 0x9d2bdc9fdea163d3: /* "flip" */
			/* TODO */
			return;
		/* ... and finally some custom commands for debugging. Unstable! */
		case 0x9acc2558707a5edd: /* "olia" (Open LIchess Analysis) */
			engine_uci_call_openlichessanalysis(engine, cmd);
			break;
		default:
			ENGINE_DEBUGF(engine, "[ERROR] Unknown command: '%s'\n", token);
	}
}
