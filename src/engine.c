#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
//#include <tensorflow/c/c_api.h>
#include "xxhash.h"
#include "engine.h"
#include "chessboard.h"
#include "util.h"

#define UCI_NEW_GAME 0xdfd89a3bb7b15ce5
#define UCI_IS_READY 0x33e37def10e5d195
#define UCI_POSITION 0x8a7ecc4137c6f2b0
#define UCI_GO 0xd682e29388e0f4a3
#define UCI_STOP 0xd8d95334d91f61fc
#define UCI_PONDER_HIT 0xe13f0de8ec4aa474
#define UCI_QUIT 0x707db5f765aed6d8
#define UCI_SET_OPTION 0x26cc87cdbb3247ba
#define UCI_REGISTER 0x6f84e39db7f2df07

struct Engine {
	bool debug;
	struct Chessboard cb;
	enum Status status;
};

struct Engine *
engine_init() {
	return &(struct Engine) {
		.debug = false,
		//.cb = *CB_INIT,
		.status = STATUS_BOOT,
	};
}

void
engine_establish_connection(struct Engine *engine) {
	// TODO
}

void
engine_wait_for_command(struct Engine *engine) {
	char *command_string = malloc(1024);
	read_line(command_string);
	char *command = strtok(command_string, " ");
	char *args[] = {};
	int args_i = 0;
	do {
		args[args_i] = strtok(command_string, " ");
	} while (args[args_i++]);
	if (!command) {
		return;
	}
	switch ((uint64_t)(XXH64(command, strlen(command), 0))) {
		case UCI_NEW_GAME:
			uci_new_game(args);
		case UCI_POSITION:
			uci_position(args);
		case UCI_GO:
			uci_go(args);
		case UCI_STOP:
			uci_stop(args);
	}
}

int
engine_wait(struct Engine *engine) {
	while (true) {
		engine_wait_for_command(engine);
		if (engine->status == STATUS_SHUTDOWN) {
			return 0;
		}
	}
}

void
uci_identify() {
	printf("id name Z64C\n");
	printf("id author Filippo Costa (@neysofu)\n");
}

void
uci_ok() {
	printf("uciok\n");
}

void
uci_ready_ok() {
	printf("readyok\n");
}

void
uci_new_game() {
}

void
uci_position() {
}

void
uci_go(struct Engine engine, int args, char *argv[]) {
}

void
uci_stop(struct Engine *engine) {
	 // TODO
}

void
uci_best_move(struct Engine engine) {
}
