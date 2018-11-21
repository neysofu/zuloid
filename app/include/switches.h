/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @file switches.h
 * @brief Compile-time flags.
 */

#pragma once

/**
 * Activate debug print statements and assertions.
 */
#define SWITCH_LOG 1

#define SWITCH_LAX_JSONRPC 1

/**
 * Turn on OpenMP optimizations for multithreading. Recommended.
 */
#define SWITCH_OPENMP 0

/// To choose the most performing GPU library on a certain system.
#define SWITCH_BACKEND_AF 0
#define SWITCH_BACKEND_CPU 1
#define SWITCH_BACKEND Z64C_SWITCH_BACKEND_CPU

/**
 * Make available training the neural network via unsupervised algorithms.
 */
#define SWITCH_TRAIN 1

/**
 * Place an upper limit to 'max_cache_size'.
 */
#define SWITCH_MAX_CACHE_SIZE (1024 * 1024 * 1024)

/**
 * Size of contigouos memory chunks in the transpositions table.
 */
#define SWITCH_TTABLE_BLOCK_SIZE (64 * 1024 * 1024)