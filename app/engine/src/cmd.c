#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmd.h"
#include "utils.h"

#if SWITCH_DEBUG
#define CMD_ARGS_DEFAULT_CAPACITY 1
#define CMD_OPTIONS_DEFAULT_CAPACITY 1
#else
#define CMD_ARGS_DEFAULT_CAPACITY 4
#define CMD_OPTIONS_DEFAULT_CAPACITY 2
#endif

struct Option {
	char *name;
	char *value;
};

struct Cmd {
	char *str;
	size_t args_num;
	size_t args_capacity;
	char *(*args)[];
	size_t options_num;
	size_t options_capacity;
	struct Option (*options)[];
};

struct Cmd *
cmd_new(void) {
	struct Cmd *cmd = malloc(sizeof(struct Cmd));
	if (!cmd) {
		return NULL;
	}
	cmd->str = NULL;
	cmd->args_num = 0;
	cmd->args_capacity = CMD_ARGS_DEFAULT_CAPACITY;
	cmd->args = malloc(sizeof(char *) * CMD_ARGS_DEFAULT_CAPACITY);
	// TODO
	cmd->options_num = 0;
	cmd->options_capacity = CMD_OPTIONS_DEFAULT_CAPACITY;
	cmd->args = malloc(sizeof(struct Option) * CMD_OPTIONS_DEFAULT_CAPACITY);
	// TODO
	return cmd;
}

void
cmd_drop(struct Cmd *cmd) {
	if (!cmd) {
		return;
	}
	free(cmd->args);
	free(cmd->options);
	free(cmd);
}

int8_t
cmd_parse(struct Cmd *cmd, char *str) {
	assert(cmd);
	assert(str);
	// TODO: implement quoting, escape character, and options.
	if (cmd->args_capacity > CMD_ARGS_DEFAULT_CAPACITY) {
		cmd->args_capacity = CMD_ARGS_DEFAULT_CAPACITY;
		cmd->args = realloc(cmd->args,
				            sizeof(char *) * CMD_ARGS_DEFAULT_CAPACITY);
	}
	if (cmd->options_capacity > CMD_OPTIONS_DEFAULT_CAPACITY) {
		cmd->options_capacity = CMD_OPTIONS_DEFAULT_CAPACITY;
		cmd->options = realloc(cmd->options,
							   sizeof(struct Option) *
							   CMD_OPTIONS_DEFAULT_CAPACITY);
	}
	cmd->str = str;
	cmd->args_num = 0;
	cmd->options_num = 0;
	size_t str_length = strlen(cmd->str);
	char *token_start;
	char *token_end = str;
	while (true) {
		token_start = token_end + strspn(token_end, WHITESPACE_CHARS);
		token_end = strpbrk(token_start, WHITESPACE_CHARS);
		if (cmd->args_num == cmd->args_capacity) {
			cmd->args_capacity <<= 1;
			cmd->args = realloc(cmd->args, cmd->args_capacity);
		}
		if (token_end) {
			(*cmd->args)[cmd->args_num++] = token_start;
			*(token_end++) = '\0';
			char *p;
			for (p = token_start; *p; ++p) {
				*p = tolower(*p);
			}
		} else {
			return 0;
		}
	}
}

char *
cmd_arg_ith(struct Cmd *cmd, size_t i) {
	if (i >= cmd->args_num) {
		return NULL;
	} else {
		return (*cmd->args)[i];
	}
}

char *
cmd_option_value(struct Cmd *cmd, char *option) {
	size_t i;
	while (i < cmd->options_num) {
		if (strcmp((*cmd->options)[i].name, option) == 0) {
			return (*cmd->options)[i].name;
		}
		i++;
	}
	return NULL;
}
