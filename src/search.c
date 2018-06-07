#include <stdio.h>
#include "search.h"
#include "uci.h"

//#include <tensorflow/c/c_api.h>
/*
int
mcts(struct Chessboard cb, struct TF_Session tf_s, strcut TF_Session tf_decoder) {
	TF_SessionRun()
	int i;

	for (i=0; i<)
}
*/

struct SearchParams *
uci_cmd_to_search_params(struct UciCmd *cmd, struct SearchParams *params) {
	/*uint16_t i = 0;
	while (i < cmd->argc) {
		swtch (cmd->argv[i]) {
			case UCI_XXH64_GO_SEARCHMOVES:
				break;
			case UCI_XXH64_GO_PONDER:
				params->ponder = true;
			case UCI_XXH64_GO_WTIME:
				params->white_time = atoi(cmd->argv[i+1]);
			case UCI_XXH64_GO_BTIME:
				params->black_time = atoi(cmd->argv[i+1]);
			case UCI_XXH64_GO_WINC:
				params->white_increment_exists = true;
				params->white_increment = atoi(cmd->argv[i+1]);
			case UCI_XXH64_GO_BINC:
				params->black_increment_exists = true;
				params->black_increment = atoi(cmd->argv[i+1]);
			case UCI_XXH64_GO_MOVESTOGO:
				params->moves_to_go = atoi(cmd->argv[i+1]);
				break;
			case UCI_XXH64_GO_INFINITE:
				params->infinite = true;
		}
	}*/
	// TODO
	return NULL;
}
