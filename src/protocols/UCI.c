/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

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
		ENGINE_LOGF(engine, "The engine is already searching.");
		return;
	}
	char *token = NULL;
	while ((token = strtok_whitespace(NULL))) {
		ENGINE_LOGF(engine, "Next token is %s\n", token);
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
				ENGINE_LOGF(engine, "Unknown argument to the GO command: '%s'\n", token);
		}
	}
	engine_start_search(engine);
}

void
engine_uci_call_position(struct Engine *engine, char *cmd)
{
	char *token = strtok_whitespace(NULL);
	if (!token) {
		return;
	} else if (strcmp(token, "startpos") == 0) {
		engine->position = POSITION_INIT;
	} else if (strcmp(token, "fen") == 0) {
		position_init_from_fen(&engine->position, cmd);
	}
	while ((token = strtok_whitespace(NULL))) {
		struct Move mv;
		string_to_move(token, &mv);
		// TODO
	}
}

void
engine_uci_call_setoption(struct Engine *engine, char *cmd)
{
	if (engine->mode != MODE_IDLE) {
		ENGINE_LOGF(engine, "Options can only be set when idle.\n");
		return;
	}
	uint64_t hash = 0;
	char *token = NULL;
	while ((token = strtok_whitespace(NULL))) {
		if (strcmp(token, "value") == 0) {
			return;
		}
		for (size_t i = 0; token[i]; i++) {
			token[i] = tolower(token[i]);
		}
		/* XOR combine the hashes. Simple yet effective. */
		hash ^= XXH64(token, strlen(token), 0);
	}
	switch (hash) {
		case 0xd8cdd8e8314c4147: /* "hash" */
			break;
		case 0xc057a61296095806: /* "nalimovpath" */
			break;
		case 0xcfaf77aca1f8feaa: /* "nalimovcache" */
			break;
		case 0x0a6f394a3987568a: /* "ponder" */
			engine->ponder = true;
		case 0x487e0e93e2c2bb18: /* "ownbook" */
			break;
		case 0xc8922f8e470ffaee: /* "uci_showcurrline" */
			break;
		case 0xdf718624bc46e2ce: /* "uci_showrefutations" */
			break;
		case 0xf6253b92eb36d560: /* "uci_limitstrength" */
			break;
		default:
			ENGINE_LOGF(engine, "Unknown UCI setting.\n");
	}
}

void
engine_uci(struct Engine *engine, char *cmd)
{
	assert(engine);
	assert(cmd);
	char *token = strtok_whitespace(cmd);
	if (!token) {
		return;
	}
	switch (XXH64(token, strlen(token), 0)) {
		case 0x5000d8f2907d14e4: /* "d" */
			position_print(&engine->position);
			break;
		case 0x01fd51a2a6f9cc2f: /* "debug" */
			token = strtok_whitespace(NULL);
			if (!token) {
				break;
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
			       Z64C_VERSION,
			       Z64C_COPYRIGHT);
			break;
		case 0xdfd89a3bb7b15ce5: /* "ucinewgame" */
			cache_clear(engine->cache);
			break;
		default:
			printf("Unknown command: %s\n", token);
	}
}
