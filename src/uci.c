#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "uci.h"

#define UCI_CMD_SEPARATOR ' '
#define UCI_CMD_ARGV_BUF_SIZE 16

struct UciCmd *
uci_cmd_new(struct UciCmd *cmd) {
	if (!cmd) {
		cmd = malloc(sizeof(struct UciCmd) +
				     sizeof(size_t) * UCI_CMD_ARGV_BUF_SIZE);
		if (!cmd) {
			return NULL;
		}
	}
	cmd->argv_size = UCI_CMD_ARGV_BUF_SIZE;
	cmd->argc = 0;
	cmd->str = NULL;
	memset(cmd->argv_by_offset, 0, cmd->argv_size);
	return cmd;
}

struct UciCmd *
str_to_uci_cmd(char *str, size_t line_len, struct UciCmd *cmd) {
	bool separator_flag = true;
	size_t i = 0;
	char c;
	uci_cmd_new(cmd);
	cmd->str = str;
	for (c = str[i]; i < line_len; c = str[++i]) {
		bool c_is_separator = c == ' ' || c == '\t';
		bool c_is_end = c == '\n' || c == '\r' || c == '\0';
		if (c_is_separator) {
			if (!separator_flag) {
				cmd->argv_by_offset[++cmd->argc] = i;
				separator_flag = true;
			}
		} else if (c_is_end) {
			cmd->argv_by_offset[++cmd->argc] = i;
			break;
		} else {
			separator_flag = false;
		}
		uci_cmd_resize_if_full(cmd);
	}
	return cmd;
}

void
uci_cmd_drop(struct UciCmd *cmd) {
	free(cmd->str);
	free(cmd);
}

bool
uci_cmd_is_full(struct UciCmd *cmd) {
	return cmd->argc >= cmd->argv_size;
}

char *
uci_cmd_arg(struct UciCmd *cmd, size_t i) {
	if (i >= cmd->argc) {
		return NULL;
	}
	return cmd->str + cmd->argv_by_offset[i];
}

size_t
uci_cmd_arg_len(struct UciCmd *cmd, size_t i) {
	if (i >= cmd->argc) {
		return 0;
	}
	return cmd->argv_by_offset[i + 1] - cmd->argv_by_offset[i];
}

void
uci_cmd_resize_if_full(struct UciCmd *cmd) {
	if (!uci_cmd_is_full(cmd)) {
		return;
	}
	// TODO
}

char *
uci_cmd_arg_with_nul(struct UciCmd *cmd, size_t i) {
	cmd->str[cmd->argv_by_offset[i + 1] + 1] = '\0';
	return uci_cmd_arg(cmd, i);
}

size_t
uci_cmd_arg_end(struct UciCmd *cmd, size_t i) {
	while (i-- < cmd->str[cmd->argv_by_offset[i]] != ' ') {
	}
	return 0;
}

void
uci_cmd_arg_unset_nul(struct UciCmd *cmd, size_t i) {
	cmd->str[cmd->argv_by_offset[i + 1]] = UCI_CMD_SEPARATOR;
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
	if (iter->i >= 1) {
		*(uci_cmd_arg(iter->cmd, iter->i) - 1) = UCI_CMD_SEPARATOR;
	}
	iter->cmd->str[iter->cmd->argv_by_offset[iter->i]] = '\0';
	return uci_cmd_arg(iter->cmd, iter->i);
}

void
uci_cmd_iter_drop(struct UciCmdIter *iter) {
	free(iter);
}
