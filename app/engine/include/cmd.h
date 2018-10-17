/// @file cmd.h

#pragma once

#include <stdint.h>
#include <stdlib.h>

// The minimum number of wasted bytes that causes 'struct Cmd' to reallocate.
#define CMD_REALLOC_TRIGGER_SIZE 16

// A container for a single UCI command that ships with several parsing
// utilities. All 'struct Cmd' instances store a pointer to the original string,
// that is immediately broken into null-terminated tokens delimited by
// 'WHITESPACE_CHARS' (see utils.h). Instances also keep track of the last
// visited token to allow sequential, iterator-style access
// with 'cmd_next'. Out-of-bounds indexing is safe and always returns NULL.

struct Cmd;

struct Cmd *
cmd_new(void);

void
cmd_free(struct Cmd *cmd);

// Creates a new 'struct Cmd' using the 'cmd' buffer or allocating one if needed
// (i.e. 'cmd' is set to NULL). 'str' is the original string that will be
// modified in-place. It returns a pointer to the instance.
int8_t
cmd_parse(struct Cmd *cmd, char *str);

// Returns the next token after the last visited one or the first one if none.
char *
cmd_arg_ith(struct Cmd *cmd, size_t i);

char *
cmd_option_value(struct Cmd *cmd, char *option);
