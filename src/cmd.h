#pragma once

#define UCI_CMD_BUF_SIZE 512
#define UCI_CMD_BUF_GROWTH_RATE 2

struct UciCmd;

struct UciCmd *
uci_cmd_new(void);

void
uci_cmd_drop(struct UciCmd *cmd);

struct UciCmd *
uci_cmd_from_str(struct UciCmd *cmd, char *str, size_t size);

void
uci_cmd_add_arg(struct UciCmd *cmd, char *arg, size_t size);

void
uci_cmd_arg_merge(struct UciCmd *cmd, size_t from, size_t to);
