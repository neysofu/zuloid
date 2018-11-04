/**
 * @file switches.h
 * @brief Compile-time flags.
 */

#pragma once

/**
 * Activate debug print statements and assertions.
 */
#define SWITCH_DEBUG 1

/**
 * Turn on OpenMP optimizations for multithreading. Reccomended.
 */
#define SWITCH_OPENMP 0

/// To choose the most performing GPU library on a certain system.
#define SWITCH_BACKEND_AF 0
#define SWITCH_BACKEND_CPU 1
#define SWITCH_BACKEND Z64C_SWITCH_BACKEND_CPU

/**
 * Make available training the neural network via unsupervised algorithms.
 */
#define SWITCH_TRAIN 0

/**
 * Place an upper limit to 'max_cache_size'.
 */
#define SWITCH_MAX_CACHE_SIZE 1E+9

/**
 * Size of contigouos memory chunks in the transpositions table.
 */
#define SWITCH_TTABLE_FRAGMENT_SIZE 16777216
