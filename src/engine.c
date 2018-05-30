#include <stdio.h>
#include <stdbool.h>
//#include <tensorflow/c/c_api.h>
#include "engine.h"
#include "chessboard.h"

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
};

int
enigne_establish_connection() {
}

switch (command) {
	case UCI_NEW_GAME:
		uci_new_game(args);
	case UCI_POSITION:
		uci_position(args);
	case UCI_GO:
		uci_go(args);
	case UCI_STOP:
		uci_stop(args);
	case UCI_STOP:
		uci_stop(args);
	case UCI_STOP:
		uci_stop(args);
	case UCI_STOP:
		uci_stop(args);
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
uci_go(struct Engine engine, int args, char *argv[]) {
	if (argv[0] == "infinite") {
	}
}

void
uci_stop(struct Engine engine) {
	engine;
}

void
uci_best_move(struct Engine engine) {
}
