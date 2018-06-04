#include "uci.h"

struct UciCmd {
	size_t argc;
	char *str;
	size_t *argv_by_offset[];
};

struct UciCmd *
uci_cmd_new(void) {
	struct UciCmd *cmd = malloc(sizeof(struct UciCmd) + UCI_CMD_RATIO_BUF_SIZE_PER_BYTE);
	if (!cmd) {
		return NULL;
	}
	cmd->argc = 0;
	cmd->str = malloc(UCI_CMD_BUF_SIZE);
	cmd->argv_by_offset = { 0 };
	return cmd;
}

void
uci_cmd_drop(struct UciCmd *cmd) {
	free(cmd->str);
	free(cmd);
}

bool
uci_cmd_buf_is_full(struct UciCmd *cmd) {
	return cmd->argv_by_offset[cmd->argc-1] >= UCI_CMD_BUF_SIZE;
}

void
uci_cmd_expand_buf_if_full(struct UciCmd *cmd) {
	if (!uci_cmd_buf_is_full(cmd)) {
		return;
	}
	realloc(cmd, UCI_CMD_BUF_GROWTH_RATE);
	realloc(cmd->str, UCI_CMD_BUF_GROWTH_RATE);
}

void
uci_cmd_add_arg(struct UciCmd *cmd, char *str, uint16_t size) {
}

struct UciCmd *
uci_cmd_from_str(struct UciCmd *cmd, char *str, uint16_t size) {
	bool whitespace_flag = false;
	if (size > UINT16_MAX) { return NULL; }
	while (true) {
		char c = getchar();
		*(engine->cmd++) = c;
		if (is_space(c)) {
			if (cmd->argc == UINT16_MAX) {
				return NULL;
			}
			if (cmd->argc++ == UCI_BUF_SIZE_ARGV) {
				return NULL;
			}
			cmd->argc++;
			cmd->argv
		if (c == EOF || c == '\n' || c == '\r') {
			break;
		}
		if ((cmd->len++) == cmd->size) {
			if (cmd->size > UINT16_MAX / UCI_BUF_GROWTH_RATE) {
				return NULL;
			}
			cmd->size *= UCI_BUF_GROWTH_RATE;
			cmd->str = realloc(cmd->str, cmd->size);
			if (!cmd->str) {
				return NULL;
			}
		}
	}
	*(cmd->str) = '\0';
	cmd->str -= cmd->len;
}

char *
uci_cmd_arg(const struct UciCmd *cmd, uint16_t i) {
	cmd->str[cmd->]
}

void
uci_cmd_arg_isolate(struct UciCmd *cmd, uint16_t i) {
	cmd->str[cmd->spaces]
}

void
engine_uci_uci(void) {
	if (engine->status != STATUS_BOOT) {
		return;
	}
	printf("id name %s\n", ENGINE_NAME);
	printf("id author %s\n", ENGINE_AUTHOR);
	engine->status = STATUS_READY;
	printf("uciok\n");
}

void
engine_uci_debug(struct Engine *engine, bool debug) {
	engine->debug = debug;
}

void
engine_uci_is_ready(struct Engine *engine) {
	printf("readyok\n");
}

void
engine_uci_set_option(struct Engine *engine, char *name, char *value) {
	return;
}

void
engine_uci_register(struct Engine *engine) {
	return;
}

void
engine_uci_new_game(struct Engine *engine) {
	return;
}

void
engine_uci_position(struct Engine *engine, struct Chessboard *cb) {
	if (engine->cmd_argc == 1) { return; }
	char *format = engine_cmd_arg(0);
	if (strcmp(format, "fen") == 0) {
		cb_specify_pieces(engine->cb, engine_cmd_arg(1));
		cb_specify_active_player(engine->cb, engine_cmd_arg(2));
		cb_specify_castling_rights(engine->cb, engine_cmd_arg(3));
		cb_specify_en_passant_target(engine->cb, engine_cmd_arg(4));
		cb_specify_half_moves(engine->cb, engine_cmd_arg(5));
	} else if (strcmp(format, "startpos") == 0) {
		*(engine->cb) = CB_INIT;
	}
	uci_log("new fen position");
	uci_log(cb_to_fen(engine->cb));
}

void
engine_uci_go(struct Engine *engine, struct SearchParameters *params) {
	return;
}

void
engine_uci_stop(struct Engine *engine) {
	return;
}

void
engine_uci_ponder_hit(struct Engine *engine) {
	return;
}

void
engine_uci_quit(struct Engine *engine) {
	engine->status = STATUS_QUIT;
}
