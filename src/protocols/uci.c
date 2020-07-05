/* Resources:
 *  - http://wbec-ridderkerk.nl/html/UCIProtocol.html
 *  - https://www.chessprogramming.org/UCI
 *  - https://www.seungjaelee.com/projects/uci/ */

#include "agent.h"
#include "cache/cache.h"
#include "chess/bb.h"
#include "chess/fen.h"
#include "chess/movegen.h"
#include "chess/position.h"
#include "core.h"
#include "engine.h"
#include "globals.h"
#include "utils.h"
#include "xxHash/xxhash.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

void
uci_call_eval(struct Engine *engine, char *cmd)
{
	printf("wmaterial %f\n", position_eval_color(&engine->position, COLOR_WHITE));
	printf("bmaterial %f\n", position_eval_color(&engine->position, COLOR_BLACK));
	printf("totmaterial %f\n", position_eval(&engine->position));
}

void
uci_call_go(struct Engine *engine, char *cmd)
{
	if (engine->mode == MODE_SEARCH) {
		ENGINE_DEBUGF(engine, "[ERROR] The engine is already searching.\n");
		return;
	}
	char *token = NULL;
	while ((token = strtok_whitespace(NULL))) {
		ENGINE_DEBUGF(engine, "[INFO] The next token is '%s'.\n", token);
		switch (XXH64(token, strlen(token), 0)) {
			case 0xf34bf7d8e647f9df: /* "perft" */
				position_perft(&engine->position, atoi(strtok_whitespace(NULL)));
				return;
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
			case 0xe142606f37a3b175: // "depth"
				break;
			case 0x3323f763484dfbd4: // "mate"
				token = strtok_whitespace(NULL);
				engine->max_depth = atoi(token);
				break;
			case 0xd6accb2d47332ac7: // "nodes"
				token = strtok_whitespace(NULL);
				engine->max_nodes_count = atoi(token);
				break;
			case 0x653009b7ced43713: // "movetime"
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
uci_call_islegal(struct Engine *engine, char *cmd)
{
	char *token = strtok_whitespace(NULL);
	struct Move moves[255] = { 0 };
	struct Move mv;
	if (!token || string_to_move(token, &mv) == 0) {
		ENGINE_DEBUGF(engine, "[ERROR] Expected token with a chess move.\n");
		return;
	}
	size_t count = gen_pseudolegal_moves(moves, &engine->position);
	ENGINE_DEBUGF(engine, "[TRACE] Examining %zu legal moves...\n", count);
	for (size_t i = 0; i < count; i++) {
		if (moves[i].source == mv.source && moves[i].target == mv.target) {
			printf("1\n");
			return;
		}
	}
	printf("0\n");
}

void
uci_call_legalmoves(struct Engine *engine, char *cmd)
{
	struct Move moves[255] = { 0 };
	/* FIXME */
	size_t count = gen_legal_moves(moves, &engine->position);
	printf("%zu", count);
	char buf[8] = { '\0' };
	for (size_t i = 0; i < count; i++) {
		move_to_string(moves[i], buf);
		printf(" %s", buf);
	}
	printf("\n");
}

void
uci_call_pseudolegalmoves(struct Engine *engine, char *cmd)
{
	struct Move moves[255] = { 0 };
	size_t count = gen_pseudolegal_moves(moves, &engine->position);
	printf("%zu", count);
	char buf[8] = { '\0' };
	for (size_t i = 0; i < count; i++) {
		move_to_string(moves[i], buf);
		printf(" %s", buf);
	}
	printf("\n");
}

void
uci_call_position(struct Engine *engine, char *cmd)
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
				if (i >= 4) {
					break;
				} else {
					ENGINE_DEBUGF(engine, "[ERROR] Not enough tokens in the FEN string.\n");
					return;
				}
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
		position_do_move_and_flip(&engine->position, &mv);
	}
}

void
uci_call_d(struct Engine *engine, char *cmd) {
	char *token = strtok_whitespace(NULL);
	if (!token) {
		position_print(&engine->position);
		return;
	}
	switch (XXH64(token, strlen(token), 0)) {
		case 0x7348d0088acc7c6c: // "lichess"
			;
			char *command = malloc(64 + FEN_SIZE);
			char *fen = fen_from_position(NULL, &engine->position, '_');
			printf("https://lichess.org/analysis/standard/%s\n", fen);
			free(command);
			free(fen);
			break;
		default:
			uci_err_syntax();
			break;
	}
}

void uci_err_syntax(void) {
	puts("[ERROR] Invalid syntax.");
}

void uci_err_unspecified(void) {
	puts("[ERROR] Unspecified error.");
}

void
uci_call_setoption(struct Engine *engine, char *cmd)
{
	if (engine->mode != MODE_IDLE) {
		ENGINE_DEBUGF(engine, "[ERROR] UCI options can only be set during idle state.\n");
		return;
	}
	XXH64_hash_t hash = 0;
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
	// Option support is quite hairy and messy. I don't want to break pre-existing scripts
	// and configs originally written for other engines.
	//
	// Please see:
	//  - https://komodochess.com/Komodo-11-README.html
	//  - http://www.rybkachess.com/index.php?auswahl=Engine+parameters
	//
	// No worries in case the links above die, just search for a list of UCI settings for
	// popular chess engines. I don't commit to 100% feature parity with any engine; I just
	// try and use my better judgement.
	switch (hash) {
		case 0xd8cdd8e8314c4147: /* "hash" */
			break;
		case 0xc057a61296095806: /* "nalimovpath" */
			break;
		case 0xcfaf77aca1f8feaa: /* "nalimovcache" */
			break;
		case 0x0a6f394a3987568a: // "ponder"
			engine->ponder = true;
			break;
		case 0x487e0e93e2c2bb18: // "ownbook"
			break;
		case 0xc8922f8e470ffaee: // "uci_showcurrline"
			break;
		case 0xdf718624bc46e2ce: // "uci_showrefutations"
			break;
		case 0xf6253b92eb36d560: // "uci_limitstrength"
			break;
		// TODO: many, many more.
		default:
			ENGINE_DEBUGF(engine, "[ERROR] No such option.\n");
	}
}

XXH64_hash_t hash_of_tokens(void) {
	XXH64_hash_t hash = 0;
	char *token;
	while ((token = strtok_whitespace(NULL))) {
		hash ^= XXH64(token, strlen(token), 0);
	}
	return hash;
}

void
protocol_uci_handle(struct Engine *restrict engine, char *cmd)
{
	char *token = strtok_whitespace(cmd);
	if (!token) {
		return;
	}
	switch (XXH64(token, strlen(token), 0)) {
		case 0x01fd51a2a6f9cc2f: // "debug"
			// This command feels quite useless (in fact, Stockfish doesn't even recognize
			// it). Nevertheless, we shall offer the option to send additional evaluation
			// details with it. It does *not* control debugging information, which instead
			// gets compiled out with the NDEBUG macro.
			token = strtok_whitespace(NULL);
			if (token && strcmp(token, "on") == 0) {
				engine->debug = true;
			} else if (token && strcmp(token, "off") == 0) {
				engine->debug = false;
			} else {
				uci_err_syntax();
			}
			break;
		case 0xd682e29388e0f4a3: // "go"
			uci_call_go(engine, cmd);
			break;
		case 0x33e37def10e5d195: // "isready"
			puts("readyok");
			break;
		case 0x8a7ecc4137c6f2b0: // "position"
			uci_call_position(engine, cmd);
			break;
		case 0x707db5f765aed6d8: // "quit"
			engine->mode = MODE_EXIT;
			break;
		case 0x26cc87cdbb3247ba: // "setoption"
			uci_call_setoption(engine, cmd);
			break;
		case 0xd8d95334d91f61fc: // "stop"
			engine_stop_search(engine);
			break;
		case 0xf80028c1113b2c9c: // "uci"
			printf("id name Z64C/%s %s\n", ZORRO_BACKEND_NAME, ZORRO_VERSION);
			puts("id author Filippo Costa");
			puts("option name Clear Hash type button");
			// TODO: also implement Komodo's Drawscore option.
			puts("option name Contempt type spin default 20 min -100 max 100");
			puts("option name Hash type spin default 64 min 0 max 131072");
			puts("option name Minimum Thinking Time type spin default 20 min 0 max 5000");
			puts("option name nodestime type spin default 0 min 0 max 10000");
			puts("option name Ponder type check default false");
			puts("option name Skill Level type spin default 20 min 0 max 20");
			// See http://www.talkchess.com/forum3/viewtopic.php?start=0&t=42308 */
			puts("option name Slow Mover type spin default 84 min 10 max 1000");
			puts("option name Threads type spin default 1 min 1 max 512");
			puts("option name Move Overhead type spin default 30 min 0 max 60000");
			puts("uciok");
			break;
		case 0xf184f2ae1a578956: // "ucidoc"
			puts("http://wbec-ridderkerk.nl/html/UCIProtocol.html");
			break;
		case 0xdfd89a3bb7b15ce5: // "ucinewgame"
			cache_clear(engine->cache);
			break;
		// The following are non-standard commands implemented in Stockfish.
		case 0x5000d8f2907d14e4: // "d"
			uci_call_d(engine, cmd);
			break;
		case 0x9d2bdc9fdea163d3: // "flip"
			// TODO
			break;
		// ... and finally some custom commands for debugging. Unstable!
		case 0xb54897ad8727b265: /* "_eval" */
			uci_call_eval(engine, cmd);
			break;
		case 0xb0582e3fa59cef0a: /* "_islegal" */
			uci_call_islegal(engine, cmd);
			break;
		case 0x96cbd35a489446bb: /* "_lm" */
			uci_call_legalmoves(engine, cmd);
			break;
			break;
		case 0x54c29874021f8627: /* "_plm" (PseudoLegal Moves) */
			uci_call_pseudolegalmoves(engine, cmd);
			break;
		case 0x32dd38952c4bc720: // "xxhash"
			printf("0x%llx\n", hash_of_tokens());
			break;
		default:
			ENGINE_DEBUGF(engine, "[ERROR] Unknown command: '%s'.\n", token);
	}
}
