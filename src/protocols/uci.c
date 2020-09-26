/* SPDX-License-Identifier: GPL-3.0-only */

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
#include "core/eval.h"
#include "engine.h"
#include "meta.h"
#include "protocols/cecp.h"
#include "protocols/support/err.h"
#include "protocols/support/pstate.h"
#include "protocols/support/uci_option.h"
#include "rating.h"
#include "switches.h"
#include "utils.h"
#include "xxHash/xxhash.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int
engine_set_ponder(struct Engine *engine, bool val)
{
	engine->config.ponder = val;
	return 0;
}

int
engine_set_contempt(struct Engine *engine, long val)
{
	engine->config.contempt = (float)(val - 100) / 200.0;
	return 0;
}

int
engine_set_skill_level(struct Engine *engine, long val)
{
	engine->config.contempt = 1.00;
	return 0;
}

int
engine_set_hash(struct Engine *engine, long val)
{
	// TODO
	return 0;
}

/* Option support is quite hairy and messy. I don't want to break pre-existing
 * scripts and configs originally written for other engines.
 *
 * Please see:
 *  - https://komodochess.com/Komodo-11-README.html
 *  - http://www.rybkachess.com/index.php?auswahl=Engine+parameters
 */
static const struct UciOption UCI_OPTIONS[] = {
	{ .name = "Analysis Contempt",
	  .type = UCI_OPTION_TYPE_COMBO,
	  .data.string = { .default_val = "Both",
	                   .combo_variants = "[Off][White][Black][Both]" } },
	{ .name = "Clear Hash",
	  .type = UCI_OPTION_TYPE_BUTTON,
	  .data.button = { .setter = NULL } },
	{ .name = "Contempt",
	  .type = UCI_OPTION_TYPE_SPIN,
	  .data.spin = { .default_val = 24, .min = -100, .max = 100 } },
	{ .name = "Debug Log File",
	  .type = UCI_OPTION_TYPE_STRING,
	  .data.string = { .default_val = "/tmp/zuloid-tmp" } },
	{ .name = "Hash",
	  .type = UCI_OPTION_TYPE_SPIN,
	  .data
	    .spin = { .default_val = 64, .min = 0, .max = 131072, .setter = engine_set_hash } },
	{ .name = "Minimum Thinking Time",
	  .type = UCI_OPTION_TYPE_SPIN,
	  .data.spin = { .default_val = 20, .min = 0, .max = 5000 } },
	{ .name = "Move Overhead",
	  .type = UCI_OPTION_TYPE_SPIN,
	  .data.spin = { .default_val = 30, .min = 30, .max = 60000 } },
	{ .name = "nodestime",
	  .type = UCI_OPTION_TYPE_SPIN,
	  .data.spin = { .default_val = 0, .min = 0, .max = 10000 } },
	{ .name = "Ponder",
	  .type = UCI_OPTION_TYPE_CHECK,
	  .data.check = { .default_val = false, .setter = engine_set_ponder } },
	{ .name = "Skill Level",
	  .type = UCI_OPTION_TYPE_SPIN,
	  .data.spin = { .default_val = 20,
	                 .min = 0,
	                 .max = 20,
	                 .setter = engine_set_skill_level } },
	{ .name = "Slow Mover",
	  .type = UCI_OPTION_TYPE_SPIN,
	  .data.spin = { .default_val = 84, .min = 10, .max = 1000 } },
	{ .name = "SyzygyPath",
	  .type = UCI_OPTION_TYPE_STRING,
	  .data.string = { .default_val = "<empty>" } },
	{ .name = "SyzygyProbeDepth",
	  .type = UCI_OPTION_TYPE_SPIN,
	  .data.spin = { .default_val = 1, .min = 1, .max = 100 } },
	{ .name = "Syzygy50MoveRule",
	  .type = UCI_OPTION_TYPE_CHECK,
	  .data.check = { .default_val = true } },
	{ .name = "SyzygyProbeLimit",
	  .type = UCI_OPTION_TYPE_SPIN,
	  .data.spin = { .default_val = 7, .min = 0, .max = 7 } },
	{ .name = "Threads",
	  .type = UCI_OPTION_TYPE_SPIN,
	  .data.spin = { .default_val = 1, .min = 1, .max = 512 } },
	{ .name = "UCI_Chess960",
	  .type = UCI_OPTION_TYPE_CHECK,
	  .data.check = { .default_val = false } },
	{ .name = "UCI_Elo",
	  .type = UCI_OPTION_TYPE_SPIN,
	  .data.spin = { .default_val = 1350, .min = 1350, .max = 2850 } },
	{ .name = "UCI_LimitStrength",
	  .type = UCI_OPTION_TYPE_CHECK,
	  .data.check = { .default_val = false } },
};

extern void
engine_call_cecp_xboard(struct Engine *engine, struct PState *pstate);

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
			token = pstate_next(pstate);
			engine->config.max_depth = atoi(token);
		} else if (strcmp(token, "mate") == 0) {
			engine_call_uci_go_mate(engine, pstate_next(pstate));
		} else if (strcmp(token, "nodes") == 0) {
			engine_call_uci_go_mate(engine, pstate_next(pstate));
		} else if (strcmp(token, "movetime") == 0) {
			engine_call_uci_go_movetime(engine, pstate_next(pstate));
		} else {
			ENGINE_LOGF(engine, "# Unrecognized 'go' option '%s'\n", token);
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
		const char *fen_fields[6] = { NULL };
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
	if (pstate_skip(pstate, "moves") == -1) {
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
	const char *name = NULL;
	if (pstate_skip(pstate, "name") != 1 || !(name = pstate_next_sep(pstate, "value"))) {
		display_err_syntax(engine->config.output);
		return;
	}
	const char *value = pstate_next_all(pstate);
	if (!value) {
		value = "";
	}
	const struct UciOption *option =
	  ucioption_find(UCI_OPTIONS, ARRAY_SIZE(UCI_OPTIONS), name);
	if (!option) {
		display_err_unspecified(engine->config.output);
		return;
	}
	switch (option->type) {
		case UCI_OPTION_TYPE_BUTTON:
			option->data.button.setter(engine);
			break;
		case UCI_OPTION_TYPE_CHECK:
			if (strcmp(value, "true") == 0) {
				option->data.check.setter(engine, true);
			} else if (strcmp(value, "false") == 0) {
				option->data.check.setter(engine, false);
			} else {
				display_err_syntax(engine->config.output);
			}
			break;
		case UCI_OPTION_TYPE_COMBO:
			if (ucioption_combo_allows(option, value)) {
				option->data.string.setter(engine, value);
			} else {
				display_err_unspecified(engine->config.output);
			}
			break;
		case UCI_OPTION_TYPE_STRING:
			option->data.string.setter(engine, value);
			break;
		case UCI_OPTION_TYPE_SPIN:;
			char *endptr = NULL;
			long val = strtol(value, &endptr, 10);
			if (endptr && val >= option->data.spin.min && val <= option->data.spin.max) {
				option->data.spin.setter(engine, val);
			} else if (!endptr) {
				display_err_syntax(engine->config.output);
			} else {
				display_err_unspecified(engine->config.output);
			}
			break;
		default:
			exit(EXIT_FAILURE);
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
	engine->config.protocol = engine_call_uci;
	fprintf(engine->config.output,
	        "id name Zuloid %s\n"
	        "id author Filippo Costa\n",
	        ZULOID_VERSION);
	for (size_t i = 0; i < ARRAY_SIZE(UCI_OPTIONS); i++) {
		ucioption_fprint(UCI_OPTIONS + i, engine->config.output);
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
engine_call_uci_ucinewgame(struct Engine *engine, struct PState *pstate)
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

const struct PCommand UCI_COMMANDS[] = {
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

void
engine_call_uci(struct Engine *engine, const char *str)
{
	struct PState *pstate = pstate_new(str, UCI_COMMANDS, ARRAY_SIZE(UCI_COMMANDS));
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
