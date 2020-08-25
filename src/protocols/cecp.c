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
#include "protocols/utils.h"
#include "rating.h"
#include "utils.h"
#include "xxHash/xxhash.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

extern void
uci_call_uci(struct Engine *engine);

void
cecp_call_xboard(struct Engine *engine, struct PState *pstate)
{
	UNUSED(pstate);
	engine->config.protocol = protocol_cecp;
}

const char *CECP_FEATURES[] = {
	"done=0",    "ping=1",   "playother=1", "debug=1",    "exclude=1",
	"nps=0",     "pause=1",  "memory=1",    "smp=1",      "usermove=1",
	"exclude=1", "sigint=0", "sigterm=0",   "setboard=1", "reuse=0",
};

void
cecp_call_protover(struct Engine *engine, struct PState *pstate)
{
	UNUSED(pstate);
	fputs("feature", engine->config.output);
	for (size_t i = 0; i < ARRAY_SIZE(CECP_FEATURES); i++) {
		fprintf(engine->config.output, " %s", CECP_FEATURES[i]);
	}
	fputc('\n', engine->config.output);
	fputs("feature done=1\n", engine->config.output);
}

void
cecp_call_ping(struct Engine *engine, struct PState *pstate)
{
	UNUSED(pstate);
	const char *token = pstate_next(pstate);
	if (token) {
		fprintf(engine->config.output, "pong %d\n", atoi(token));
	}
}

void
cecp_call_quit(struct Engine *engine, struct PState *pstate)
{
	UNUSED(pstate);
	engine->status = STATUS_EXIT;
}

void
cecp_call_playother(struct Engine *engine, struct PState *pstate)
{
	UNUSED(pstate);
	engine->board.side_to_move = color_other(engine->board.side_to_move);
}

void
cecp_call_setboard(struct Engine *engine, struct PState *pstate)
{
	const char *token = pstate_next(pstate);
	if (!token) {
		display_err_syntax(engine->config.output);
		return;
	}
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
}

void
cecp_call_draw(struct Engine *engine, struct PState *pstate)
{
	UNUSED(pstate);
	fputs("offer draw\n", engine->config.output);
}

void
cecp_call_result(struct Engine *engine, struct PState *pstate)
{
	UNUSED(pstate);
	engine->status = STATUS_IDLE;
}

void
cecp_call_new(struct Engine *engine, struct PState *pstate)
{
	UNUSED(pstate);
	position_init_from_fen(&engine->board, FEN_OF_INITIAL_POSITION);
}

void
cecp_call_post(struct Engine *engine, struct PState *pstate)
{
	UNUSED(pstate);
	engine->config.debug = true;
}

void
cecp_call_nopost(struct Engine *engine, struct PState *pstate)
{
	UNUSED(pstate);
	engine->config.debug = false;
}

void
cecp_call_hard(struct Engine *engine, struct PState *pstate)
{
	UNUSED(pstate);
	engine->config.ponder = true;
}

void
cecp_call_easy(struct Engine *engine, struct PState *pstate)
{
	UNUSED(pstate);
	engine->config.ponder = false;
}

void
cecp_call_ratings(struct Engine *engine, struct PState *pstate)
{
	UNUSED(engine);
	UNUSED(pstate);
	// TODO
}

void
cecp_call_pause(struct Engine *engine, struct PState *pstate)
{
	UNUSED(engine);
	UNUSED(pstate);
	// TODO
}

void
cecp_call_resume(struct Engine *engine, struct PState *pstate)
{
	UNUSED(engine);
	UNUSED(pstate);
	// TODO
}

void
cecp_call_usermove(struct Engine *engine, struct PState *pstate)
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
cecp_call_question_mark(struct Engine *engine, struct PState *pstate)
{
	UNUSED(engine);
	UNUSED(pstate);
	// TODO: find and print a move.
}

void
cecp_call_time(struct Engine *engine, struct PState *pstate)
{
	UNUSED(engine);
	UNUSED(pstate);
}

void
cecp_call_otim(struct Engine *engine, struct Pstate *pstate)
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
cecp_call_sd(struct Engine *engine, struct PState *pstate)
{
	const char *token = pstate_next(pstate);
	int quantity = atoi(token);
	engine->config.max_depth = quantity;
}

void
cecp_call_exit(struct Engine *engine, struct PState *pstate)
{
	engine->status = STATUS_IDLE;
	engine_stop_search(engine);
}

void
cecp_call_analyze(struct Engine *engine, struct PState *pstate)
{
	engine_start_search(engine);
}

void
cecp_call_cores(struct Engine *engine, struct PState *pstate)
{
	UNUSED(engine);
	UNUSED(pstate);
	// TODO
}

void
cecp_call_exclude(struct Engine *engine, struct PState *pstate)
{
	UNUSED(engine);
	UNUSED(pstate);
	// TODO
}

void
cecp_call_include(struct Engine *engine, struct PState *pstate)
{
	UNUSED(engine);
	UNUSED(pstate);
	// TODO
}

void
cecp_call_memory(struct Engine *engine, struct PState *pstate)
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
cecp_call_random(struct Engine *engine, struct PState *pstate)
{
	UNUSED(pstate);
	engine->config.move_selection_noise = 0.5;
}

const struct Command CECP_COMMANDS[] = {
	{ "?", cecp_call_question_mark },
	{ "analyze", cecp_call_analyze },
	{ "cores", cecp_call_cores },
	{ "draw", cecp_call_draw },
	{ "easy", cecp_call_easy },
	{ "exclude", cecp_call_exclude },
	{ "exit", cecp_call_exit },
	{ "hard", cecp_call_hard },
	{ "include", cecp_call_include },
	{ "memory", cecp_call_memory },
	{ "new", cecp_call_new },
	{ "nopost", cecp_call_nopost },
	{ "otim", cecp_call_otim },
	{ "pause", cecp_call_pause },
	{ "ping", cecp_call_ping },
	{ "playother", cecp_call_playother },
	{ "post", cecp_call_post },
	{ "protover", cecp_call_protover },
	{ "quit", cecp_call_quit },
	{ "random", cecp_call_random },
	{ "ratings", cecp_call_ratings },
	{ "result", cecp_call_result },
	{ "resume", cecp_call_resume },
	{ "sd", cecp_call_sd },
	{ "setboard", cecp_call_setboard },
	{ "time", cecp_call_time },
	{ "uci", uci_call_uci },
	{ "xboard", cecp_call_xboard },
};

void
protocol_cecp(struct Engine *engine, const char *str)
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
