#pragma once

#include <stdint.h>
#include <stdlib.h>

// The minimum number of wasted bytes that causes 'struct Cmd' to reallocate.
#define CMD_REALLOC_TRIGGER_SIZE 32

// A container for a single UCI command that ships with several parsing
// utilities. All 'struct Cmd' instances store a pointer to the original string,
// that is immediately broken into null-terminated tokens delimited by
// 'WHITESPACE_CHARS' (see utils.h). Instances also keep track of the last
// visited token to allowi sequential, iterator-style access
// with 'cmd_next'. Out-of-bounds indexing is safe and always returns NULL.
struct Cmd;

// Creates a new 'struct Cmd' using the 'cmd' buffer or allocating one if needed
// (i.e. 'cmd' is set to NULL). 'str' is the original string that will be
// modified in-place. It returns a pointer to the instance.
struct Cmd *
cmd_from_str(struct Cmd *cmd, char *str);

// Creates a new 'struct Cmd' using the 'cmd' buffer or allocating one if needed
// (i.e. 'cmd' is set to NULL). The first line (\n) from 'stream' is read and
// used as a source. It returns a pointer to the instance, or NULL if reading a
// line from 'straem' failed.
struct Cmd *
cmd_from_stream(struct Cmd *cmd, FILE *stream);

// Frees this instance. Please note that it does NOT free the original string.
void
cmd_drop(struct Cmd *cmd);

// Returns the 'i'th token (with 0-based indexing) or NULL if nonexistant.
char *
cmd_ith(struct Cmd *cmd, size_t i);

// Returns the next token after the last visited one or the first one if none.
char *
cmd_next(struct Cmd *cmd);

// Returns the last visited token or 
char *
cmd_current(struct Cmd *cmd);

// Returns the token just before the last visited one or NULL if none.
char *
cmd_previous(struct Cmd *cmd);

// Calls 'cmd_next' a number of times and discards the results.
void
cmd_skip(struct Cmd *cmd, size_t n);

// Joins te
// Temporarily join 'n' tokens starting at 'i' with spaces (0x20).
char *
cmd_join(struct Cmd *cmd, size_t n);

// Disjoins 'n' tokens starting at 'i' after joining them.
char *
cmd_disjoin(struct Cmd *cmd, size_t n);
