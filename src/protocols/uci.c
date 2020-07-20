// Resources:
//  - http://wbec-ridderkerk.nl/html/UCIProtocol.html
//  - https://www.chessprogramming.org/UCI
//  - https://www.seungjaelee.com/projects/uci/

#include "protocols/uci.h"
#include "agent.h"
#include "cache/cache.h"
#include "chess/bb.h"
#include "chess/fen.h"
#include "chess/find_magics.h"
#include "chess/movegen.h"
#include "chess/position.h"
#include "core.h"
#include "engine.h"
#include "globals.h"
#include "rating.h"
#include "utils.h"
#include "xxHash/xxhash.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

void
uci_err_syntax(FILE *stream)
{
	fputs("[ERROR] Invalid syntax.\n", stream);
}

void
uci_err_unspecified(FILE *stream)
{
	fputs("[ERROR] Unspecified error.\n", stream);
}

void
uci_err_invalid_command(FILE *stream)
{
	fputs("[ERROR] Invalid command.\n", stream);
}

int
uci_command_cmp(const void *cmd1, const void *cmd2)
{
	const char *s1 = ((struct UciCommand *)cmd1)->name;
	const char *s2 = ((struct UciCommand *)cmd2)->name;
	return strcmp(s1, s2);
}

struct UciCommand *
uci_identify_command(const char *token)
{
	struct UciCommand key = {
		.name = token,
		.handler = NULL,
	};
	return bsearch(
	  &key, UCI_COMMANDS, UCI_COMMANDS_COUNT, sizeof(struct UciCommand), uci_command_cmp);
}

void
protocol_uci_handle(struct Engine *engine, const char *s_const)
{
	char *s = exit_if_null(malloc(strlen(s_const) + 1));
	strcpy(s, s_const);
	char *token = strtok_whitespace(s);
	struct UciCommand *cmd = NULL;
	if (token && (cmd = uci_identify_command(token))) {
		ENGINE_DEBUGF(engine, "Accepted UCI command.\n");
		cmd->handler(engine, s);
	} else if (token) {
		uci_err_invalid_command(engine->output);
	}
	free(s);
}

void
uci_call_eval(struct Engine *engine, char *cmd)
{
	fprintf(
	  engine->output, "wmaterial %f\n", position_eval_color(&engine->board, COLOR_WHITE));
	fprintf(
	  engine->output, "bmaterial %f\n", position_eval_color(&engine->board, COLOR_BLACK));
	fprintf(engine->output, "totmaterial %f\n", position_eval(&engine->board));
}

void
uci_call_go(struct Engine *engine, char *s)
{
	if (engine->status != STATUS_IDLE) {
		return;
	}
	char *cmd = exit_if_null(malloc(strlen(s) + 1));
	strcpy(cmd, s);
	char *token = NULL;
	while ((token = strtok_whitespace(NULL))) {
		switch (djb_hash(token)) {
			case 3430: // "perft"
				token = strtok_whitespace(NULL);
				if (token) {
					position_perft(engine->output, &engine->board, atoi(token));
				} else {
					uci_err_syntax(engine->output);
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
uci_call_islegal(struct Engine *engine, char *cmd)
{
	char *token = strtok_whitespace(NULL);
	struct Move moves[255] = { 0 };
	struct Move mv;
	if (!token || string_to_move(token, &mv) == 0) {
		ENGINE_DEBUGF(engine, "[ERROR] Expected token with a chess move.\n");
		return;
	}
	size_t count = gen_pseudolegal_moves(moves, &engine->board);
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
uci_call_pseudolegalmoves(struct Engine *engine, char *cmd)
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
uci_call_export_magics(struct Engine *engine, char *cmd)
{
	char *filename = strtok_whitespace(NULL);
	if (!filename) {
		uci_err_syntax(engine->output);
		return;
	}
	FILE *file = fopen(filename, "w");
	if (!file) {
		uci_err_unspecified(engine->output);
		return;
	}
	fprintf(file, "-- ROOK MAGICS:\n");
	for (Square sq = 0; sq <= SQUARE_MAX; sq++) {
		fprintf(file, "0x%zu:\n", MAGICS[sq].multiplier);
	}
	fclose(file);
}

void
uci_call_position(struct Engine *engine, char *cmd)
{
	char *token = strtok_whitespace(NULL);
	if (!token) {
		uci_err_syntax(engine->output);
		return;
	} else if (strcmp(token, "startpos") == 0) {
		engine->board = POSITION_INIT;
	} else if (strcmp(token, "fen") == 0) {
		char *fen_fields[6] = { NULL };
		for (size_t i = 0; i < 6; i++) {
			token = strtok_whitespace(NULL);
			if (!token) {
				if (i >= 4) {
					break;
				} else {
					uci_err_syntax(engine->output);
					return;
				}
			}
			fen_fields[i] = token;
		}
		position_init_from_fen_fields(&engine->board, fen_fields);
	} else if (strcmp(token, "960") == 0) {
		/* The "960" command is a custom addition the standard. I figured it could be useful
		 * for training. */
		position_init_960(&engine->board);
	} else if (strcmp(token, "current") != 0) {
		uci_err_syntax(engine->output);
	}
	if ((token = strtok_whitespace(NULL)) && strcmp(token, "moves")) {
		uci_err_syntax(engine->output);
	}
	// Now feed moves into the position.
	while ((token = strtok_whitespace(NULL))) {
		struct Move mv;
		string_to_move(token, &mv);
		position_do_move_and_flip(&engine->board, &mv);
	}
}

void
uci_call_d(struct Engine *engine, char *cmd)
{
	char *token = strtok_whitespace(NULL);
	char *fen = NULL;
	if (!token) {
		position_print(engine->output, &engine->board);
	} else if (streq(token, "fen")) {
		fen = fen_from_position(NULL, &engine->board, ' ');
		fprintf(engine->output, "%s\n", fen);
		free(fen);
	} else if (streq(token, "lichess")) {
		fen = fen_from_position(NULL, &engine->board, '_');
		fprintf(engine->output, "https://lichess.org/analysis/standard/%s\n", fen);
		free(fen);
	} else {
		uci_err_syntax(engine->output);
	}
}

void
uci_call_setoption(struct Engine *engine, char *cmd)
{
	if (engine->status != STATUS_IDLE) {
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
		// XOR combine the hashes. Simple yet effective.
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
			ENGINE_DEBUGF(engine, "[ERROR] No such option.\n");
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
	// TODO
}

void
uci_call_uci(struct Engine *engine)
{
	fprintf(engine->output,
	        "id name Zorro %s\n"
	        "id author Filippo Costa\n"
	        "id elo %u\n",
	        ZORRO_VERSION,
	        CCRL_4015_RATING);
	for (size_t i = 0; i < UCI_OPTIONS_COUNT; i++) {
		fputs(UCI_OPTIONS[i], engine->output);
		putc('\n', engine->output);
	}
	bb_init();
	fputs("uciok\n", engine->output);
}

void
uci_call_ucinewgame(struct Engine *engine)
{
	// TODO
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
uci_call_debug(struct Engine *engine, char *cmd)
{
	// This command feels quite useless (in fact, Stockfish doesn't even recognize
	// it). Nevertheless, we shall offer the option to send additional evaluation
	// details with it. It does *not* control debugging information, which instead
	// gets compiled out with the NDEBUG macro.
	char *token = strtok_whitespace(NULL);
	if (token && strcmp(token, "on") == 0) {
		engine->debug = true;
	} else if (token && strcmp(token, "off") == 0) {
		engine->debug = false;
	} else {
		uci_err_syntax(engine->output);
	}
}

const struct UciCommand UCI_COMMANDS[] = {
	{ "_eval", uci_call_eval },
	{ "_islegal", uci_call_islegal },
	{ "_lm", uci_call_legalmoves },
	{ "_plm", uci_call_pseudolegalmoves },
	{ "d", uci_call_d },
	{ "debug", uci_call_debug },
	{ "djbhash", uci_call_djbhash },
	{ "go", uci_call_go },
	{ "isready", uci_call_isready },
	{ "magics", uci_call_export_magics },
	{ "position", uci_call_position },
	{ "quit", uci_call_quit },
	{ "setoption", uci_call_setoption },
	{ "stop", uci_call_stop },
	{ "uci", uci_call_uci },
	{ "ucinewgame", uci_call_ucinewgame },
};

const size_t UCI_COMMANDS_COUNT = sizeof(UCI_COMMANDS) / sizeof(UCI_COMMANDS[0]);

const char *UCI_OPTIONS[] = {
	"option name Analysis Contempt type combo default Both var Off var White var Black var "
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

const size_t UCI_OPTIONS_COUNT = sizeof(UCI_OPTIONS) / sizeof(UCI_OPTIONS[0]);
