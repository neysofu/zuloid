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
#include "core.h"
#include "engine.h"
#include "globals.h"
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
cecp_call_xboard(struct Engine *engine)
{
	engine->protocol = protocol_cecp;
}

const char *CECP_FEATURES[] = {
	"done=0",    "ping=1",   "playother=1", "debug=1",    "exclude=1",
	"nps=0",     "pause=1",  "memory=1",    "smp=1",      "usermove=1",
	"exclude=1", "sigint=0", "sigterm=0",   "setboard=1", "reuse=0",
};

void
cecp_call_protover(struct Engine *engine)
{
	fputs("feature", engine->output);
	for (size_t i = 0; i < ARRAY_SIZE(CECP_FEATURES); i++) {
		fprintf(engine->output, " %s", CECP_FEATURES[i]);
	}
	fputc('\n', engine->output);
	fputs("feature done=1\n", engine->output);
}

void
cecp_call_ping(struct Engine *engine)
{
	char *token = strtok_whitespace(NULL);
	fprintf(engine->output, "pong %d\n", atoi(token));
}

void
cecp_call_quit(struct Engine *engine)
{
	engine->status = STATUS_EXIT;
}

void
cecp_call_playother(struct Engine *engine)
{
	engine->board.side_to_move = color_other(engine->board.side_to_move);
}

void
cecp_call_setboard(struct Engine *engine)
{
	char *token = strtok_whitespace(NULL);
	if (!token) {
		display_err_syntax(engine->output);
		return;
	}
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
}

void
cecp_call_draw(struct Engine *engine)
{
	fputs("offer draw\n", engine->output);
}

void
cecp_call_result(struct Engine *engine)
{
	engine->status = STATUS_IDLE;
}

void
cecp_call_new(struct Engine *engine)
{
	engine->board = POSITION_INIT;
}

void
cecp_call_post(struct Engine *engine)
{
	engine->debug = true;
}

void
cecp_call_nopost(struct Engine *engine)
{
	engine->debug = false;
}

void
cecp_call_hard(struct Engine *engine)
{
	engine->ponder = true;
}

void
cecp_call_easy(struct Engine *engine)
{
	engine->ponder = false;
}

void
cecp_call_ratings(struct Engine *engine)
{
	UNUSED(engine);
	// TODO
}

void
cecp_call_pause(struct Engine *engine)
{
	UNUSED(engine);
	// TODO
}

void
cecp_call_resume(struct Engine *engine)
{
	UNUSED(engine);
	// TODO
}

void
cecp_call_usermove(struct Engine *engine)
{
	char *token = strtok_whitespace(NULL);
	struct Move mv;
	string_to_move(token, &mv);
	position_do_move_and_flip(&engine->board, &mv);
}

void
cecp_call_question_mark(struct Engine *engine)
{
	UNUSED(engine);
	// TODO: find and print a move.
}

void
cecp_call_time(struct Engine *engine)
{
	UNUSED(engine);
}

void
cecp_call_otim(struct Engine *engine)
{
	char *token = strtok_whitespace(NULL);
	float time_in_secs = (float)(atoi(token)) / 100.0;
	engine->game_clocks[engine->board.side_to_move].time_left_in_seconds = time_in_secs;
}

void
cecp_call_sd(struct Engine *engine)
{
	char *token = strtok_whitespace(NULL);
	int quantity = atoi(token);
	engine->max_depth = quantity;
}

void
cecp_call_exit(struct Engine *engine)
{
	engine->status = STATUS_IDLE;
	engine_stop_search(engine);
}

void
cecp_call_analyze(struct Engine *engine)
{
	engine_start_search(engine);
}

void
cecp_call_cores(struct Engine *engine)
{
	UNUSED(engine);
	// TODO
}

void
cecp_call_exclude(struct Engine *engine)
{
	UNUSED(engine);
	// TODO
}

void
cecp_call_include(struct Engine *engine)
{
	UNUSED(engine);
	// TODO
}

void
cecp_call_memory(struct Engine *engine)
{
	cache_delete(engine->cache);
	char *token = strtok_whitespace(NULL);
	size_t memory_in_bytes = atoi(token) * 1024;
	engine->cache = cache_new(memory_in_bytes);
}

void
cecp_call_random(struct Engine *engine)
{
	engine->move_selection_noise = 0.5;
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
protocol_cecp(struct Engine *engine, const char *s_const)
{
	char *s = exit_if_null(malloc(strlen(s_const) + 1));
	strcpy(s, s_const);
	const char *token = strtok_whitespace(s);
	const struct Command *cmd = NULL;
	if (!token) {
		;
	} else if ((cmd = identify_command(token, CECP_COMMANDS, ARRAY_SIZE(CECP_COMMANDS)))) {
		ENGINE_LOGF(engine, "Accepted CECP command.\n");
		cmd->handler(engine);
	} else if (string_represents_coordinate_notation_move(token)) {
		struct Move move;
		string_to_move(token, &move);
		position_do_move_and_flip(&engine->board, &move);
	} else {
		display_err_invalid_command(engine->output);
	}
	free(s);
}
