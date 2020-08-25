// Resources:
//  - http://wbec-ridderkerk.nl/html/UCIProtocol.html
//  - https://www.chessprogramming.org/UCI
//  - https://www.seungjaelee.com/projects/uci/

#include "protocols/uci.h"
#include "agent.h"
#include "cache/cache.h"
#include "chess/bb.h"
#include "chess/fen.h"
#include "chess/magic.h"
#include "chess/movegen.h"
#include "chess/position.h"
#include "chess/threats.h"
#include "engine.h"
#include "meta.h"
#include "protocols/utils.h"
#include "rating.h"
#include "switches.h"
#include "utils.h"
#include "xxHash/xxhash.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

extern void
engine_call_cecp_xboard(struct Engine *engine);

void
engine_call_uci_eval(struct Engine *engine, struct PState *pstate)
{
	UNUSED(pstate);
	fprintf(engine->config.output,
	        "wmaterial %f\n",
	        position_eval_color(&engine->board, COLOR_WHITE));
	fprintf(engine->config.output,
	        "bmaterial %f\n",
	        position_eval_color(&engine->board, COLOR_BLACK));
	fprintf(engine->config.output, "totmaterial %f\n", position_eval(&engine->board));
}

void
engine_call_uci_go_perft(struct Engine *engine, const char *arg)
{
	if (arg) {
		position_perft(engine->config.output, &engine->board, atoi(arg));
	} else {
		display_err_syntax(engine->config.output);
	}
}

void
engine_call_uci_go_time(struct Engine *engine, const char *arg, enum Color color)
{
	if (arg) {
		float time = (float)(atol(arg) * 1000);
		engine->time_controls[color]->time_limit_in_seconds = time;
	} else {
		display_err_syntax(engine->config.output);
	}
}

void
engine_call_uci_go_inc(struct Engine *engine, const char *arg, enum Color color)
{
	if (arg) {
		float time = (float)(atol(arg) * 1000);
		engine->time_controls[color]->increment_in_seconds = time;
	} else {
		display_err_syntax(engine->config.output);
	}
}

void
engine_call_uci_go_mate(struct Engine *engine, const char *token)
{
	if (token) {
		engine->config.max_depth = atoi(token);
	} else {
		display_err_syntax(engine->config.output);
	}
}

void
engine_call_uci_go_movetime(struct Engine *engine, const char *token)
{
	if (token) {
		float time = (float)(atol(token) * 1000);
		engine->game_clocks[engine->board.side_to_move].time_left_in_seconds = time;
	} else {
		display_err_syntax(engine->config.output);
	}
}

void
engine_call_uci_go(struct Engine *engine, struct PState *pstate)
{
	if (engine->status != STATUS_IDLE) {
		return;
	}
	const char *token = NULL;
	while ((token = pstate_next(pstate))) {
		if (strcmp(token, "perft") == 0) {
			engine_call_uci_go_perft(engine, pstate_next(pstate));
			return;
		} else if (strcmp(token, "wtime") == 0) {
			engine_call_uci_go_time(engine, pstate_next(pstate), COLOR_WHITE);
		} else if (strcmp(token, "btime") == 0) {
			engine_call_uci_go_time(engine, pstate_next(pstate), COLOR_BLACK);
		} else if (strcmp(token, "winc") == 0) {
			engine_call_uci_go_inc(engine, pstate_next(pstate), COLOR_WHITE);
		} else if (strcmp(token, "binc") == 0) {
			engine_call_uci_go_inc(engine, pstate_next(pstate), COLOR_BLACK);
		} else if (strcmp(token, "infinite") == 0) {
			time_control_delete(engine->time_controls[COLOR_WHITE]);
			time_control_delete(engine->time_controls[COLOR_BLACK]);
		} else if (strcmp(token, "ponder") == 0) {
			engine->config.ponder = true;
		} else if (strcmp(token, "movestogo") == 0) {
			token = pstate_next(pstate);
			engine->time_controls[COLOR_WHITE]->max_moves_count = atoi(token);
			engine->time_controls[COLOR_BLACK]->max_moves_count = atoi(token);
		} else if (strcmp(token, "depth") == 0) {
			token = strtok_whitespace(NULL);
			engine->config.max_depth = atoi(token);
		} else if (strcmp(token, "mate") == 0) {
			engine_call_uci_go_mate(engine, pstate_next(pstate));
		} else if (strcmp(token, "nodes") == 0) {
			engine_call_uci_go_mate(engine, pstate_next(pstate));
		} else if (strcmp(token, "movetime") == 0) {
			engine_call_uci_go_movetime(engine, pstate_next(pstate));
		}
	}
	engine_start_search(engine);
}

void
engine_call_uci_listmoves(struct Engine *engine, struct PState *pstate)
{
	const char *token = pstate_next(pstate);
	size_t count = 0;
	struct Move moves[255] = { 0 };
	if (strcmp(token, "pseudolegal") == 0) {
		count = gen_pseudolegal_moves(moves, &engine->board);
	} else if (strcmp(token, "legal") == 0) {
		count = gen_legal_moves(moves, &engine->board);
	} else {
		display_err_syntax(engine->config.output);
		return;
	}
	fprintf(engine->config.output, "%zu", count);
	for (size_t i = 0; i < count; i++) {
		char buf[8] = { '\0' };
		move_to_string(moves[i], buf);
		fprintf(engine->config.output, " %s", buf);
	}
	putc('\n', engine->config.output);
}

void
engine_call_uci_position(struct Engine *engine, struct PState *pstate)
{
	const char *token = pstate_next(pstate);
	if (!token) {
		display_err_syntax(engine->config.output);
		return;
	} else if (strcmp(token, "startpos") == 0) {
		engine->board = POSITION_INIT;
	} else if (strcmp(token, "fen") == 0) {
		char *fen_fields[6] = { NULL };
		for (size_t i = 0; i < 6; i++) {
			token = pstate_next(pstate);
			if (!token) {
				if (i >= 4) {
					break;
				} else {
					display_err_syntax(engine->config.output);
					return;
				}
			}
			fen_fields[i] = token;
		}
		position_init_from_fen_fields(&engine->board, fen_fields);
	} else if (strcmp(token, "960") == 0) {
		/* The "960" command is a custom addition the standard. I figured it could
		 * be useful for training. */
		position_init_960(&engine->board);
	} else if (strcmp(token, "current") != 0) {
		display_err_syntax(engine->config.output);
	}
	if ((token = pstate_next(pstate)) && strcmp(token, "moves") != 0) {
		display_err_syntax(engine->config.output);
		return;
	}
	// Now feed moves into the position.
	while ((token = pstate_next(pstate))) {
		struct Move mv;
		string_to_move(token, &mv);
		position_do_move_and_flip(&engine->board, &mv);
	}
}

void
engine_call_uci_d(struct Engine *engine, struct PState *pstate)
{
	const char *token = pstate_next(pstate);
	if (!token) {
		position_pprint(&engine->board, engine->config.output);
	} else if (strcmp(token, "fen") == 0) {
		char *fen = fen_from_position(NULL, &engine->board, ' ');
		fprintf(engine->config.output, "%s\n", fen);
		free(fen);
	} else if (strcmp(token, "lichess") == 0) {
		char *fen = fen_from_position(NULL, &engine->board, '_');
		fprintf(engine->config.output, "https://lichess.org/analysis/standard/%s\n", fen);
		free(fen);
	} else {
		display_err_syntax(engine->config.output);
	}
}

void
engine_call_uci_setoption(struct Engine *engine, struct PState *pstate)
{
	if (engine->status != STATUS_IDLE) {
		display_err_unspecified(engine->config.output);
		return;
	}
	XXH64_hash_t hash = 0;
	char *token = NULL;
	while ((token = strtok_whitespace(NULL))) {
		if (strcmp(token, "value") == 0) {
			return;
		}
		/* From the UCI protocol specification (April 2004):
		 * > The name of the option in  should not be case sensitive and can inludes
		 * spaces > like also the value. */
		for (char *ptr = token; *ptr; ptr++) {
			*ptr = tolower(*ptr);
		}
		// XOR combine the hashes. Simple yet effective.
		hash ^= XXH64(token, strlen(token), 0);
	}
	// Option support is quite hairy and messy. I don't want to break pre-existing
	// scripts and configs originally written for other engines.
	//
	// Please see:
	//  - https://komodochess.com/Komodo-11-README.html
	//  - http://www.rybkachess.com/index.php?auswahl=Engine+parameters
	//
	// No worries in case the links above die, just search for a list of UCI
	// settings for popular chess engines. I don't commit to 100% feature parity
	// with any engine; I just try and use my better judgement.
	switch (hash) {
		case 49865: // "hash"
			break;
		case 33256: // "nalimovpath"
			break;
		case 14447: // "nalimovcache"
			break;
		case 29613: // "ponder"
			engine->config.ponder = true;
			break;
		case 868: // "ownbook"
			break;
		case 37547: // "uci_showcurrline"
			break;
		case 38426: // "uci_showrefutations"
			break;
		case 61491: // "uci_limitstrength"
			break;
		// TODO: many, many more.
		default:
			ENGINE_LOGF(engine, "[ERROR] No such option.\n");
	}
}

void
engine_call_uci_isready(struct Engine *engine, struct PState *pstate)
{
	UNUSED(pstate);
	fputs("readyok\n", engine->config.output);
}

void
engine_call_uci_quit(struct Engine *engine, struct PState *pstate)
{
	UNUSED(pstate);
	engine->status = STATUS_EXIT;
}

void
engine_call_uci_stop(struct Engine *engine, struct PState *pstate)
{
	UNUSED(engine);
	UNUSED(pstate);
}

void
engine_call_uci_uci(struct Engine *engine, struct PState *pstate)
{
	UNUSED(pstate);
	const char *options[] = {
		"option name Analysis Contempt type combo default Both var Off var White "
		"var Black var Both",
		"option name Clear Hash type button",
		"option name Contempt type spin default 24 min -100 max 100",
		"option name Debug Log File type string default",
		"option name Hash type spin default 64 min 0 max 131072",
		"option name Minimum Thinking Time type spin default 20 min 0 max 5000",
		"option name Move Overhead type spin default 30 min 0 max 60000",
		"option name nodestime type spin default 0 min 0 max 10000",
		"option name Ponder type check default false",
		"option name Skill Level type spin default 20 min 0 max 20",
		"option name Slow Mover type spin default 84 min 10 max 1000",
		"option name SyzygyPath type string default <empty>",
		"option name SyzygyProbeDepth type spin default 1 min 1 max 100",
		"option name Syzygy50MoveRule type check default true",
		"option name SyzygyProbeLimit type spin default 7 min 0 max 7",
		"option name Threads type spin default 1 min 1 max 512",
		"option name UCI_Chess960 type check default false",
		"option name UCI_AnalyseMode type check default false",
		"option name UCI_LimitStrength type check default false",
		"option name UCI_Elo type spin default 1350 min 1350 max 2850",
	};
	engine->config.protocol = engine_call_uci;
	fprintf(engine->config.output,
	        "id name Zuloid %s\n"
	        "id author Filippo Costa\n",
	        ZULOID_VERSION);
	for (size_t i = 0; i < ARRAY_SIZE(options); i++) {
		fputs(options[i], engine->config.output);
		putc('\n', engine->config.output);
	}
	init_threats();
	fputs("uciok\n", engine->config.output);
}

void
engine_call_uci_magics(struct Engine *engine, struct PState *pstate)
{
	const char *token = pstate_next(pstate);
	const char *identifier = NULL;
	void (*finder)(struct Magic *, Square);
	if (!strcmp(token, "bishop")) {
		identifier = "MAGICS_BISHOP";
		finder = magic_find_bishop;
	} else if (!strcmp(token, "rook")) {
		identifier = "MAGICS_ROOK";
		finder = magic_find_rook;
	} else {
		display_err_syntax(engine->config.output);
		return;
	}
	struct Magic magics[64];
	for (Square sq = 0; sq < SQUARES_COUNT; sq++) {
		finder(magics + sq, sq);
	}
	magics_export(magics, identifier, engine->config.output);
}

void
engine_call_uci_ucinewgame(struct Engine *engine, struct Pstate *pstate)
{
	UNUSED(engine);
	UNUSED(pstate);
}

void
engine_call_uci_debug(struct Engine *engine, struct PState *pstate)
{
	// This command feels quite useless (in fact, Stockfish doesn't even recognize
	// it). Nevertheless, we shall offer the option to send additional evaluation
	// details with it. It does *not* control debugging information, which instead
	// gets compiled out with the NDEBUG macro.
	const char *token = pstate_next(pstate);
	if (token && strcmp(token, "on") == 0) {
		engine->config.debug = true;
	} else if (token && strcmp(token, "off") == 0) {
		engine->config.debug = false;
	} else {
		display_err_syntax(engine->config.output);
	}
}

void
engine_call_uci(struct Engine *engine, const char *str)
{
	const struct PCommand commands[] = {
		{ "%eval", engine_call_uci_eval },
		{ "%listmoves", engine_call_uci_listmoves },
		{ "%magics", engine_call_uci_magics },
		{ "d", engine_call_uci_d },
		{ "debug", engine_call_uci_debug },
		{ "go", engine_call_uci_go },
		{ "isready", engine_call_uci_isready },
		{ "position", engine_call_uci_position },
		{ "quit", engine_call_uci_quit },
		{ "setoption", engine_call_uci_setoption },
		{ "stop", engine_call_uci_stop },
		{ "uci", engine_call_uci_uci },
		{ "ucinewgame", engine_call_uci_ucinewgame },
		{ "xboard", engine_call_cecp_xboard },
	};
	struct PState *pstate = pstate_new(str, commands, ARRAY_SIZE(commands));
	if (!pstate->token) {
		;
	} else if (pstate->cmd) {
		ENGINE_LOGF(engine, "Accepted UCI command.\n");
		pstate->cmd->handler(engine, pstate);
	} else {
		display_err_invalid_command(engine->config.output);
	}
	pstate_free(pstate);
}
