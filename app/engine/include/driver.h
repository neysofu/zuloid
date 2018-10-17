/// @file driver.h
/// @brief The core components of Z64C.
/// This file contains the API to the very core of Z64C.

#pragma once

#include "chess/board.h"
#include "chess/legality.h"
#include "chess/result.h"
#include "chess/termination.h"
#include "cmd.h"
#include "game.h"
#include "mode.h"
#include "search/ttable.h"
#include "settings.h"
#include <stdbool.h>

#define DRIVER_NAME "Z64C"
#define DRIVER_VERSION "v0.6"
#define DRIVER_RELEASE_DATE __DATE__
#define DRIVER_AUTHOR "Filippo Costa (@neysofu)"
#define DRIVER_URL "https://zsixfourc.net"

/// @brief A self-contained chess driver instance.
/// The @c Driver structure contains all the necessary components to make Z64C
/// work.
/// This structure doesn't takes up very little memory by itself. The most
/// resource-hungry component by far and large is 'struct TTable'.
struct Driver
{
	struct TTable *ttable;
	struct Board board;
	struct Result result;
	struct Settings settings;
	struct Game *game;
	enum Mode mode;
	int8_t exit_status;
	uint64_t seed;
};

/// @brief Instantiates a new chess driver.
///
/// The newly created chess driver is
/// still asleep, without responding to commands.
///
/// @see driver_main
struct Driver *
driver_new(void);

/// @brief Gracefully kill a struct Driver instance.
///
/// @param driver A pointer to the driver instance to kill.
void
driver_free(struct Driver *driver);

/// @brief Initializes @p driver and starts listening from a ZUCI shell.
///
/// Once initialized, @p param will be actively listening on `stdin` for ZUCI
/// commands. Output information will be written to `stdout` and error messages
/// on `stderr`. This function will return only upon failure or a \a quit
/// command.
///
/// @param driver The driver instance to initialize.
/// @return A non-zero error code on failure, zero on success.
int8_t
driver_main(struct Driver *driver);

void
driver_seed(struct Driver *driver);

/// @brief Evaluates a ZUCI command.
int8_t
driver_run_str(struct Driver *driver, char *str);
