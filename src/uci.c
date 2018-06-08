#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "uci.h"

#define UCI_CMD_SEPARATOR ' '
#define UCI_CMD_ARGV_MAX 16

struct UciCmdArg {
	size_t offset;
	size_t length;
};

struct UciCmd *
uci_cmd_new(struct UciCmd *cmd) {
	size_t argv_size = sizeof(struct UciCmdArg) * UCI_CMD_ARGV_MAX;
	if (!cmd) {
		cmd = malloc(sizeof(struct UciCmd));
		if (!cmd) {
			return NULL;
		}
	}
	cmd->argc = 0;
	cmd->str = NULL;
	cmd->argv_max = UCI_CMD_ARGV_MAX;
	cmd->argv = malloc(argv_size);
	if (!cmd->argv) {
		return NULL;
	}
	memset(cmd->argv, 0, argv_size);
	return cmd;
}

void
uci_cmd_drop(struct UciCmd *cmd) {
	free(cmd->argv);
	free(cmd);
}

struct UciCmd *
str_to_uci_cmd(char *str, size_t line_len, struct UciCmd *cmd) {
	bool token_flag = false;
	bool str_is_terminated = false;
	size_t i = 0;
	char c;
	cmd = uci_cmd_new(cmd);
	cmd->str = str;
	for (c = str[i]; i < line_len; c = str[++i]) {
		bool c_is_space = isspace(c);
		if (c_is_space && token_flag) {
			cmd->argv[cmd->argc - 1].length = i - cmd->argv[cmd->argc - 1].offset;
		}
		if (c == '\n' || c == '\0') {
			break;
		}
		if (!(c_is_space || token_flag)) {
			cmd->argv[cmd->argc].offset = i;
			cmd->argc++;
		}
		token_flag = !c_is_space;
		if (!uci_cmd_resize_if_full(cmd)) {
			return NULL;
		}
	}
	return cmd;
}

char *
uci_cmd_arg(struct UciCmd *cmd, size_t i) {
	return cmd->str + cmd->argv[i].offset;
}

size_t
uci_cmd_arg_length(struct UciCmd *cmd, size_t i) {
	return cmd->argv[i].length;
}

char *
uci_cmd_arg_end(struct UciCmd *cmd, size_t i) {
	return uci_cmd_arg(cmd, i) + uci_cmd_arg_length(cmd, i);
}

struct UciCmdArg *
uci_cmd_resize_if_full(struct UciCmd *cmd) {
	if (cmd->argc == cmd->argv_max) {
		cmd->argv_max *= 2;
		size_t argv_size = sizeof(struct UciCmdArg) * cmd->argv_max;
		cmd->argv = realloc(cmd->argv, argv_size);
		if (!cmd->argv) {
			return NULL;
		}
	}
	return cmd->argv;
}

struct UciCmdIter *
uci_cmd_iter_new(struct UciCmdIter *iter, struct UciCmd *cmd, size_t start) {
	if (!iter) {
		iter = malloc(sizeof(struct UciCmdIter));
		if (!iter) {
			return NULL;
		}
	}
	iter->cmd = cmd;
	iter->i = start;
	return iter;
}

char *
uci_cmd_iter_next(struct UciCmdIter *iter) {
	char *arg;
	if (iter->i == iter->cmd->argc) {
		return NULL;
	}
	arg = uci_cmd_arg(iter->cmd, iter->i);
	*uci_cmd_arg_end(iter->cmd, iter->i) = '\0';
	if (iter->i > 0) {
		*(uci_cmd_arg_end(iter->cmd, iter->i - 1)) = UCI_CMD_SEPARATOR;
	}
	iter->i++;
	return arg;
}

void
uci_cmd_iter_drop(struct UciCmdIter *iter) {
	free(iter);
}
