// Resources:
//  - http://hgm.nubati.net/CECP.html

#include "protocols/cecp.h"
#include "agent.h"
#include "cache/cache.h"
#include "chess/bb.h"
#include "chess/fen.h"
#include "chess/magic.h"
#include "chess/movegen.h"
#include "chess/position.h"
#include "engine.h"
#include "meta.h"
#include "protocols/support/err.h"
#include "protocols/support/pstate.h"
#include "rating.h"
#include "utils.h"
#include "xxHash/xxhash.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

void
engine_call_cecp_xboard(struct Engine *engine, struct PState *pstate) {
	UNUSED(pstate);
	engine->config.protocol = engine_call_cecp;
}

extern void
engine_call_uci_uci(struct Engine *engine, struct PState *pstate);

void
engine_call_cecp_protover(struct Engine *engine, struct PState *pstate)
{
	UNUSED(pstate);
	const char *features[] = {
		"done=0",    "ping=1",   "playother=1", "debug=1",    "exclude=1",
		"nps=0",     "pause=1",  "memory=1",    "smp=1",      "usermove=1",
		"exclude=1", "sigint=0", "sigterm=0",   "setboard=1", "reuse=0",
	};
	fputs("feature", engine->config.output);
	for (size_t i = 0; i < ARRAY_SIZE(features); i++) {
		fprintf(engine->config.output, " %s", features[i]);
	}
	fputc('\n', engine->config.output);
	fputs("feature done=1\n", engine->config.output);
}

void
engine_call_cecp_ping(struct Engine *engine, struct PState *pstate)
{
	const char *token = pstate_next(pstate);
	if (!token) {
		fprintf(engine->config.output, "pong\n");
	} else {
		fprintf(engine->config.output, "pong %d\n", atoi(token));
	}
}

void
engine_call_cecp_quit(struct Engine *engine, struct PState *pstate)
{
	UNUSED(pstate);
	engine->status = STATUS_EXIT;
}

void
engine_call_cecp_playother(struct Engine *engine, struct PState *pstate)
{
	UNUSED(pstate);
	engine->board.side_to_move = color_other(engine->board.side_to_move);
}

void
engine_call_cecp_setboard(struct Engine *engine, struct PState *pstate)
{
	const char *token = pstate_next(pstate);
	if (!token) {
		display_err_syntax(engine->config.output);
		return;
	}
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
}

void
engine_call_cecp_draw(struct Engine *engine, struct PState *pstate)
{
	UNUSED(pstate);
	fputs("offer draw\n", engine->config.output);
}

void
engine_call_cecp_result(struct Engine *engine, struct PState *pstate)
{
	UNUSED(pstate);
	engine->status = STATUS_IDLE;
}

void
engine_call_cecp_new(struct Engine *engine, struct PState *pstate)
{
	UNUSED(pstate);
	position_init_from_fen(&engine->board, FEN_OF_INITIAL_POSITION);
}

void
engine_call_cecp_post(struct Engine *engine, struct PState *pstate)
{
	UNUSED(pstate);
	engine->config.debug = true;
}

void
engine_call_cecp_nopost(struct Engine *engine, struct PState *pstate)
{
	UNUSED(pstate);
	engine->config.debug = false;
}

void
engine_call_cecp_hard(struct Engine *engine, struct PState *pstate)
{
	UNUSED(pstate);
	engine->config.ponder = true;
}

void
engine_call_cecp_easy(struct Engine *engine, struct PState *pstate)
{
	UNUSED(pstate);
	engine->config.ponder = false;
}

void
engine_call_cecp_ratings(struct Engine *engine, struct PState *pstate)
{
	UNUSED(engine);
	UNUSED(pstate);
	// TODO
}

void
engine_call_cecp_pause(struct Engine *engine, struct PState *pstate)
{
	UNUSED(engine);
	UNUSED(pstate);
	// TODO
}

void
engine_call_cecp_resume(struct Engine *engine, struct PState *pstate)
{
	UNUSED(engine);
	UNUSED(pstate);
	// TODO
}

void
engine_call_cecp_usermove(struct Engine *engine, struct PState *pstate)
{
	const char *token = pstate_next(pstate);
	if (token) {
		struct Move mv;
		string_to_move(token, &mv);
		position_do_move_and_flip(&engine->board, &mv);
	} else {
		display_err_syntax(engine->config.output);
	}
}

void
engine_call_cecp_question_mark(struct Engine *engine, struct PState *pstate)
{
	UNUSED(engine);
	UNUSED(pstate);
	// TODO: find and print a move.
}

void
engine_call_cecp_time(struct Engine *engine, struct PState *pstate)
{
	UNUSED(engine);
	UNUSED(pstate);
}

void
engine_call_cecp_otim(struct Engine *engine, struct Pstate *pstate)
{
	const char *token = pstate_next(pstate);
	if (token) {
		float time_in_secs = (float)(atoi(token)) / 100.0;
		engine->game_clocks[engine->board.side_to_move].time_left_in_seconds = time_in_secs;
	} else {
		display_err_syntax(engine->config.output);
	}
}

void
engine_call_cecp_sd(struct Engine *engine, struct PState *pstate)
{
	const char *token = pstate_next(pstate);
	int quantity = atoi(token);
	engine->config.max_depth = quantity;
}

void
engine_call_cecp_exit(struct Engine *engine, struct PState *pstate)
{
	engine->status = STATUS_IDLE;
	engine_stop_search(engine);
}

void
engine_call_cecp_analyze(struct Engine *engine, struct PState *pstate)
{
	engine_start_search(engine);
}

void
engine_call_cecp_cores(struct Engine *engine, struct PState *pstate)
{
	UNUSED(engine);
	UNUSED(pstate);
	// TODO
}

void
engine_call_cecp_exclude(struct Engine *engine, struct PState *pstate)
{
	UNUSED(engine);
	UNUSED(pstate);
	// TODO
}

void
engine_call_cecp_include(struct Engine *engine, struct PState *pstate)
{
	UNUSED(engine);
	UNUSED(pstate);
	// TODO
}

void
engine_call_cecp_memory(struct Engine *engine, struct PState *pstate)
{
	cache_delete(engine->cache);
	const char *token = pstate_next(pstate);
	if (token) {
		size_t memory_in_bytes = atoi(token) * 1024;
		engine->cache = cache_new(memory_in_bytes);
	} else {
		display_err_syntax(engine->config.output);
	}
}

void
engine_call_cecp_random(struct Engine *engine, struct PState *pstate)
{
	UNUSED(pstate);
	engine->config.move_selection_noise = 0.5;
}

const struct PCommand CECP_COMMANDS[] = {
	{ "?", engine_call_cecp_question_mark },
	{ "analyze", engine_call_cecp_analyze },
	{ "cores", engine_call_cecp_cores },
	{ "draw", engine_call_cecp_draw },
	{ "easy", engine_call_cecp_easy },
	{ "exclude", engine_call_cecp_exclude },
	{ "exit", engine_call_cecp_exit },
	{ "hard", engine_call_cecp_hard },
	{ "include", engine_call_cecp_include },
	{ "memory", engine_call_cecp_memory },
	{ "new", engine_call_cecp_new },
	{ "nopost", engine_call_cecp_nopost },
	{ "otim", engine_call_cecp_otim },
	{ "pause", engine_call_cecp_pause },
	{ "ping", engine_call_cecp_ping },
	{ "playother", engine_call_cecp_playother },
	{ "post", engine_call_cecp_post },
	{ "protover", engine_call_cecp_protover },
	{ "quit", engine_call_cecp_quit },
	{ "random", engine_call_cecp_random },
	{ "ratings", engine_call_cecp_ratings },
	{ "result", engine_call_cecp_result },
	{ "resume", engine_call_cecp_resume },
	{ "sd", engine_call_cecp_sd },
	{ "setboard", engine_call_cecp_setboard },
	{ "time", engine_call_cecp_time },
	{ "uci", engine_call_uci_uci },
	{ "xboard", engine_call_cecp_xboard },
};

void
engine_call_cecp(struct Engine *engine, const char *str)
{
	struct PState *pstate = pstate_new(str, CECP_COMMANDS, ARRAY_SIZE(CECP_COMMANDS));
	if (!pstate->token) {
		;
	} else if (pstate->cmd) {
		ENGINE_LOGF(engine, "Accepted CECP command.\n");
		pstate->cmd->handler(engine, pstate);
	} else if (string_represents_coordinate_notation_move(pstate->token)) {
		struct Move move;
		string_to_move(pstate->token, &move);
		position_do_move_and_flip(&engine->board, &move);
	} else {
		display_err_invalid_command(engine->config.output);
	}
	pstate_free(pstate);
}
