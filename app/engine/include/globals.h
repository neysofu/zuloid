/// @file globals.h
/// @brief Global constants initialization.

#pragma once

#include <stdlib.h>

#define PORT 34290

/// @brief Initializes globals variables and runs @c bb_init.
/// This function can safely be called multiple times without any effect after
/// the first call.
/// @post All global variables are, from now on, constants.
void
globals_init(void);

/// @brief The system's page size in bytes.
extern size_t PAGE_SIZE;
