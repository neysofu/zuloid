// Resources:
//  - http://wbec-ridderkerk.nl/html/UCIProtocol.html
//  - https://www.chessprogramming.org/UCI
//  - https://www.seungjaelee.com/projects/uci/

#include "agent.h"
#include "cache/cache.h"
#include "chess/bb.h"
#include "chess/fen.h"
#include "chess/magic.h"
#include "chess/magic.h"
#include "chess/movegen.h"
#include "chess/position.h"
#include "core.h"
#include "engine.h"
#include "globals.h"
#include "protocols/uci.h"
#include "protocols/utils.h"
#include "rating.h"
#include "switches.h"
#include "utils.h"
#include "xxHash/xxhash.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

extern void
cecp_call_xboard(struct Engine *engine);

void
uci_call_eval(struct Engine *engine)
{
	fprintf(
	  engine->output, "wmaterial %f\n", position_eval_color(&engine->board, COLOR_WHITE));
	fprintf(
	  engine->output, "bmaterial %f\n", position_eval_color(&engine->board, COLOR_BLACK));
	fprintf(engine->output, "totmaterial %f\n", position_eval(&engine->board));
}

void
uci_call_go(struct Engine *engine)
{
	if (engine->status != STATUS_IDLE) {
		return;
	}
	char *token = NULL;
	while ((token = strtok_whitespace(NULL))) {
		switch (djb_hash(token)) {
			case 3430: // "perft"
				token = strtok_whitespace(NULL);
				if (token) {
					position_perft(engine->output, &engine->board, atoi(token));
				} else {
					display_err_syntax(engine->output);
				}
				return;
			case 52523: // "wtime"
				token = strtok_whitespace(NULL);
				engine->time_controls[COLOR_WHITE]->time_limit_in_seconds =
				  atoi(token) * 1000;
				break;
			case 51862: // "btime"
				token = strtok_whitespace(NULL);
				engine->time_controls[COLOR_BLACK]->time_limit_in_seconds =
				  atoi(token) * 1000;
				break;
			case 7638: // "winc"
				token = strtok_whitespace(NULL);
				engine->time_controls[COLOR_WHITE]->increment_in_seconds =
				  atoi(token) * 1000;
				break;
			case 26541: // "binc"
				token = strtok_whitespace(NULL);
				engine->time_controls[COLOR_BLACK]->increment_in_seconds =
				  atoi(token) * 1000;
				break;
			case 4603: // "infinite"
				time_control_delete(engine->time_controls[COLOR_WHITE]);
				time_control_delete(engine->time_controls[COLOR_BLACK]);
				break;
			case 29613: // "ponder"
				engine->ponder = true;
				break;
			case 33256: // "movestogo"
				token = strtok_whitespace(NULL);
				engine->time_controls[COLOR_WHITE]->max_moves_count = atoi(token);
				engine->time_controls[COLOR_BLACK]->max_moves_count = atoi(token);
				break;
			case 57498: // "depth"
				break;
			case 32972: // "mate"
				token = strtok_whitespace(NULL);
				engine->max_depth = atoi(token);
				break;
			case 7294: // "nodes"
				token = strtok_whitespace(NULL);
				engine->max_nodes_count = atoi(token);
				break;
			case 5035: // "movetime"
				token = strtok_whitespace(NULL);
				engine->game_clocks[engine->board.side_to_move].time_left_in_seconds =
				  atoi(token) * 1000;
				break;
		}
	}
	engine_start_search(engine);
}

void
uci_call_islegal(struct Engine *engine)
{
	struct Move moves[255] = { 0 };
	struct Move mv;
	char *token = strtok_whitespace(NULL);
	if (!token || string_to_move(token, &mv) == 0) {
		display_err_syntax(engine->output);
		return;
	}
	size_t count = gen_legal_moves(moves, &engine->board);
	for (size_t i = 0; i < count; i++) {
		if (moves[i].source == mv.source && moves[i].target == mv.target) {
			fputs("1\n", engine->output);
			return;
		}
	}
	fputs("0\n", engine->output);
}

void
uci_call_legalmoves(struct Engine *engine)
{
	struct Move moves[255] = { 0 };
	/* FIXME */
	size_t count = gen_legal_moves(moves, &engine->board);
	fprintf(engine->output, "%zu", count);
	char buf[8] = { '\0' };
	for (size_t i = 0; i < count; i++) {
		move_to_string(moves[i], buf);
		fprintf(engine->output, " %s", buf);
	}
	fprintf(engine->output, "\n");
}

void
uci_call_pseudolegalmoves(struct Engine *engine)
{
	struct Move moves[255] = { 0 };
	size_t count = gen_pseudolegal_moves(moves, &engine->board);
	fprintf(engine->output, "%zu", count);
	char buf[8] = { '\0' };
	for (size_t i = 0; i < count; i++) {
		move_to_string(moves[i], buf);
		fprintf(engine->output, " %s", buf);
	}
	putc('\n', engine->output);
}

void
uci_call_position(struct Engine *engine)
{
	char *token = strtok_whitespace(NULL);
	if (!token) {
		display_err_syntax(engine->output);
		return;
	} else if (streq(token, "startpos")) {
		engine->board = POSITION_INIT;
	} else if (streq(token, "fen")) {
		char *fen_fields[6] = { NULL };
		for (size_t i = 0; i < 6; i++) {
			token = strtok_whitespace(NULL);
			if (!token) {
				if (i >= 4) {
					break;
				} else {
					display_err_syntax(engine->output);
					return;
				}
			}
			fen_fields[i] = token;
		}
		position_init_from_fen_fields(&engine->board, fen_fields);
	} else if (streq(token, "960")) {
		/* The "960" command is a custom addition the standard. I figured it could
		 * be useful for training. */
		position_init_960(&engine->board);
	} else if (!streq(token, "current")) {
		display_err_syntax(engine->output);
	}
	if ((token = strtok_whitespace(NULL)) && !streq(token, "moves")) {
		display_err_syntax(engine->output);
		return;
	}
	// Now feed moves into the position.
	while ((token = strtok_whitespace(NULL))) {
		struct Move mv;
		string_to_move(token, &mv);
		position_do_move_and_flip(&engine->board, &mv);
	}
}

void
uci_call_d(struct Engine *engine)
{
	char *token = strtok_whitespace(NULL);
	if (!token) {
		position_pprint(&engine->board, engine->output);
	} else if (streq(token, "fen")) {
		char *fen = fen_from_position(NULL, &engine->board, ' ');
		fprintf(engine->output, "%s\n", fen);
		free(fen);
	} else if (streq(token, "lichess")) {
		char *fen = fen_from_position(NULL, &engine->board, '_');
		fprintf(engine->output, "https://lichess.org/analysis/standard/%s\n", fen);
		free(fen);
	} else {
		display_err_syntax(engine->output);
	}
}

void
uci_call_setoption(struct Engine *engine)
{
	if (engine->status != STATUS_IDLE) {
		display_err_unspecified(engine->output);
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
			engine->ponder = true;
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
uci_call_isready(struct Engine *engine)
{
	fputs("readyok\n", engine->output);
}

void
uci_call_quit(struct Engine *engine)
{
	engine->status = STATUS_EXIT;
}

void
uci_call_stop(struct Engine *engine)
{
	UNUSED(engine);
}

const char *UCI_OPTIONS[] = {
	"option name Analysis Contempt type combo default Both var Off var White "
	"var Black var "
	"Both",
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

void
uci_call_uci(struct Engine *engine)
{
	engine->protocol = protocol_uci;
	fprintf(engine->output,
	        "id name Zuloid %s\n"
	        "id author Filippo Costa\n",
	        ZULOID_VERSION);
	for (size_t i = 0; i < ARRAY_SIZE(UCI_OPTIONS); i++) {
		fputs(UCI_OPTIONS[i], engine->output);
		putc('\n', engine->output);
	}
	bb_init();
	magic_init();
	fputs("uciok\n", engine->output);
}

void
uci_call_magics(struct Engine *engine)
{
	char *token = strtok_whitespace(NULL);
	const char *identifier = NULL;
	void (*finder)(struct Magic *, Square);
	if (streq(token, "bishop")) {
		identifier = "MAGICS_BISHOP";
		finder = magic_find_bishop;
	} else if (streq(token, "rook")) {
		identifier = "MAGICS_ROOK";
		finder = magic_find_rook;
	} else {
		display_err_syntax(engine->output);
		return;
	}
	struct Magic magics[64];
	for (Square sq = 0; sq < SQUARES_COUNT; sq++) {
		finder(magics + sq, sq);
	}
	magics_export(magics, identifier, engine->output);
}

void
uci_call_ucinewgame(struct Engine *engine)
{
	UNUSED(engine);
}

void
uci_call_djbhash(struct Engine *engine)
{
	uint16_t hash = 0;
	char *token = NULL;
	while ((token = strtok_whitespace(NULL))) {
		hash ^= djb_hash(token);
	}
	fprintf(engine->output, "%u\n", hash);
}

void
uci_call_debug(struct Engine *engine)
{
	// This command feels quite useless (in fact, Stockfish doesn't even recognize
	// it). Nevertheless, we shall offer the option to send additional evaluation
	// details with it. It does *not* control debugging information, which instead
	// gets compiled out with the NDEBUG macro.
	const char *token = strtok_whitespace(NULL);
	if (token && streq(token, "on")) {
		engine->debug = true;
	} else if (token && strcmp(token, "off")) {
		engine->debug = false;
	} else {
		display_err_syntax(engine->output);
	}
}

const struct Command UCI_COMMANDS[] = {
	{ "%eval", uci_call_eval },
	{ "%islegal", uci_call_islegal },
	{ "%lm", uci_call_legalmoves },
	{ "%magics", uci_call_magics },
	{ "%plm", uci_call_pseudolegalmoves },
	{ "d", uci_call_d },
	{ "debug", uci_call_debug },
	{ "djbhash", uci_call_djbhash },
	{ "go", uci_call_go },
	{ "isready", uci_call_isready },
	{ "position", uci_call_position },
	{ "quit", uci_call_quit },
	{ "setoption", uci_call_setoption },
	{ "stop", uci_call_stop },
	{ "uci", uci_call_uci },
	{ "ucinewgame", uci_call_ucinewgame },
	{ "xboard", cecp_call_xboard },
};

void
protocol_uci(struct Engine *engine, const char *s_const)
{
	char *s = exit_if_null(malloc(strlen(s_const) + 1));
	strcpy(s, s_const);
	const char *token = strtok_whitespace(s);
	const struct Command *cmd = NULL;
	if (!token) {
		;
	} else if ((cmd = identify_command(token, UCI_COMMANDS, ARRAY_SIZE(UCI_COMMANDS)))) {
		ENGINE_LOGF(engine, "Accepted UCI command.\n");
		cmd->handler(engine);
	} else {
		display_err_invalid_command(engine->output);
	}
	free(s);
}
