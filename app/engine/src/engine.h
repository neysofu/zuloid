#pragma once

#include <stdbool.h>
#include "board.h"
#include "cmd.h"
#include "move_stack.h"
#include "mover.h"
#include "result.h"
#include "saves_dir.h"
#include "settings.h"
#include "status.h"
#include "ttable.h"
#include "termination.h"

#define ENGINE_NAME "Z64C"
#define ENGINE_VERSION "v0.6"
#define ENGINE_RELEASE_DATE "2018-09-04"
#define ENGINE_AUTHOR "Filippo Costa (@neysofu)"
#define ENGINE_WEBSITE "https://zsixfourc.net"

// A working and self-contained chess engine with no dependencies to any
// external components except for global constants.
// stdout. A Z64C instance can only focus on one chess position at the time, so
// an user that wants to analyze multiple positions will have to use separate
// engine instances.
struct Engine {
	struct TTable *ttable;
	struct Board board;
	struct Result result;
	struct Mover mover;
	struct MoveStack *move_stack;
	struct Cmd *cmd;
	enum Status status;
	struct Settings settings;
	struct SavesDir *saves_dir;
};

// Creates and then returns a new instance of the Z64C chess engine. The engine
// will not yet be listening for UCI commands on stdin and its 'Status' will be
// 'STATUS_BOOT'.
struct Engine *
engine_new(void);

// Calls all deconstructors on 'engine' and finally frees it. Please note that
// is does not write saves to disk.
void
engine_drop(struct Engine *engine);

// Starts a UCI session on the three standard streams (stdin, stdout, and
// stderr) until told to stop by using the 'quit' command. It then returns
// 'EX_OK' as an exit code.
uint_fast8_t
engine_main(struct Engine *engine);

// Evaluates the UCI command 'cmd'.
void
engine_run(struct Engine *engine, char *cmd);
