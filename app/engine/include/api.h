/// @file api.h
/// @brief C API for ZUCI commands.

#pragma once

#include <stdlib.h>
#include "cmd.h"
#include "driver.h"

// The following functions are the entry execution points for specific ZUCI
// commands.
/// @pre Both @p driver and @p cmd point to two valid structures instantiated
/// with @c driver_new and @c cmd_new, respectively.

int8_t
driver_call_foo(struct Driver *driver, struct Cmd *cmd);

int8_t
driver_call_load(struct Driver *driver, struct Cmd *cmd);

int8_t
driver_call_move(struct Driver *driver, struct Cmd *cmd);

int8_t
driver_call_print(struct Driver *driver, struct Cmd *cmd);

int8_t
driver_call_purge(struct Driver *driver, struct Cmd *cmd);

int8_t
driver_call_quit(struct Driver *driver, struct Cmd *cmd);

int8_t
driver_call_save(struct Driver *driver, struct Cmd *cmd);

int8_t
driver_call_set(struct Driver *driver, struct Cmd *cmd);

int8_t
driver_call_setup(struct Driver *driver, struct Cmd *cmd);

int8_t
driver_call_think(struct Driver *driver, struct Cmd *cmd);
