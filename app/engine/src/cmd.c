#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmd.h"
#include "utils.h"

struct Cmd {
	// May NOT be set to NULL.
	char *str;
	size_t str_len;
	// Normally set to the index of the last visited token, but set to
	// 'num_tokens' if the last visited token is out of bounds.
	size_t last_token_i;
	size_t num_tokens;
	char *tokens[];
};

struct Cmd *
cmd_from_str(struct Cmd *cmd, char *str) {
	size_t num_tokens = 0;
	char *strpbrk_ptr = str;
	while ((strpbrk_ptr = strpbrk(strpbrk_ptr, WHITESPACE_CHARS)) != NULL) {
		num_tokens++;
	}
	if (cmd) {
		if (cmd->num_tokens < num_tokens ||
			cmd->num_tokens - num_tokens > CMD_REALLOC_TRIGGER_SIZE) {
			cmd = realloc(cmd, sizeof(struct Cmd) + sizeof(char *) * num_tokens);
		}
	} else {
		cmd = malloc(sizeof(struct Cmd) + sizeof(char *) * num_tokens);
	}
	cmd->str = str;
	cmd->str_len = strlen(str);
	cmd->last_token_i = num_tokens;
	cmd->num_tokens = num_tokens;
	return cmd;
}

struct Cmd *
cmd_from_stream(struct Cmd *cmd, FILE *stream) {
	char *str;
	size_t str_len;
	if (getline(&str, &str_len, stream) == -1) {
		return NULL;
	}
	return cmd_from_str(cmd, str);
}

void
cmd_drop(struct Cmd *cmd) {
	free(cmd);
}

char *
cmd_nth(struct Cmd *cmd, size_t n) {
	cmd->last_token_i = n % cmd->num_tokens;
	return cmd->tokens[n % cmd->num_tokens];
}

char *
cmd_next(struct Cmd *cmd) {
	return cmd_nth(cmd, cmd->last_token_i + 1);
}

void
cmd_skip(struct Cmd *cmd, size_t i) {
	while (i-- > 0) {
		cmd_next(cmd);
	}
}

char *
cmd_previous(struct Cmd *cmd) {
	return cmd_nth(cmd, cmd->last_token_i - 1);
}
