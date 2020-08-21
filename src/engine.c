#include "engine.h"
#include "agent.h"
#include "cache/cache.h"
#include "chess/fen.h"
#include "chess/position.h"
#include "mt-64/mt-64.h"
#include "protocols/uci.h"
#include "utils.h"
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const struct Config CONFIG_DEFAULT;

void
init_subsystems(void)
{
	init_genrand64(0xd895643full);
	p_libsys_init();
	// UCI (and CECP as well, for that matter) is a line-oriented protocol; so
	// we want to turn line buffering on.
	setvbuf(stdin, NULL, _IOLBF, 0);
	setvbuf(stdout, NULL, _IOLBF, 0);
}

struct Engine *
engine_new(void)
{
	struct Engine *engine = exit_if_null(malloc(sizeof(struct Engine)));
	engine_init(engine);
	return engine;
}

void
engine_init(struct Engine *engine)
{
	*engine = (struct Engine){
		.time_controls = { time_control_new_bullet(), time_control_new_bullet() },
		.cache = NULL,
		.agent = agent_new(),
		.seed = 0xcfca130b,
		.status = STATUS_IDLE,
		.config = CONFIG_DEFAULT,
	};
	engine->config.output = stdout;
	position_init_from_fen(&engine->board, FEN_OF_INITIAL_POSITION);
}

void
engine_delete(struct Engine *engine)
{
	time_control_delete(engine->time_controls[COLOR_WHITE]);
	time_control_delete(engine->time_controls[COLOR_BLACK]);
	cache_delete(engine->cache);
	agent_delete(engine->agent);
	free(engine);
}

void
engine_logf(struct Engine *engine,
            const char *filename,
            size_t line_num,
            const char *function_name,
            ...)
{
	assert(engine);
	assert(filename);
	assert(function_name);
	if (!engine->config.debug) {
		return;
	}
	fprintf(engine->config.output, "info string ");
	fprintf(engine->config.output, "%s:%zu @ %s -- ", filename, line_num, function_name);
	va_list args;
	va_start(args, function_name);
	vfprintf(engine->config.output, va_arg(args, const char *), args);
	va_end(args);
}

const struct Config CONFIG_DEFAULT = {
	.debug = false,
	.move_selection_noise = 0.005,
	.contempt = 0.3,
	.selectivity = 0.5,
	.ponder = false,
	.max_nodes_count = 0,
	.max_depth = 0,
	.protocol = protocol_uci,
	.output = NULL,
};

