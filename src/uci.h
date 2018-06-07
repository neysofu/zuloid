#pragma once

#include "board.h"
#include "move.h"

struct UciCmd {
	size_t argc;
	char *str;
	size_t argv_size;
	size_t argv_by_offset[];
};

struct UciCmd *
uci_cmd_new(struct UciCmd *cmd);

void
uci_cmd_drop(struct UciCmd *cmd);

struct UciCmd *
str_to_uci_cmd(char *str, size_t len, struct UciCmd *cmd);

bool
uci_cmd_is_full(struct UciCmd *cmd);

void
uci_cmd_resize_if_full(struct UciCmd *cmd);

char *
uci_cmd_arg(struct UciCmd *cmd, size_t i);

size_t
uci_cmd_arg_len(struct UciCmd *cmd, size_t i);

char *
uci_cmd_arg_set_nul(struct UciCmd *cmd, size_t i);

void
uci_cmd_arg_unset_nul(struct UciCmd *cmd, size_t i);

void
uci_cmd_arg_join(struct UciCmd *cmd, size_t i);

void
uci_cmd_add_arg(struct UciCmd *cmd, char *arg);

struct UciCmdIter {
	struct UciCmd *cmd;
	size_t i;
};

struct UciCmdIter *
uci_cmd_iter_new(struct UciCmdIter *iter, struct UciCmd *cmd, size_t start);

char *
uci_cmd_iter_next(struct UciCmdIter *iter);

void
uci_cmd_iter_drop(struct UciCmdIter *iter);
