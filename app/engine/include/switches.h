/// @file switches.h
/// @brief Compile-time flags.

#pragma once

#define SWITCH_DEBUG 0

/// To choose the most performing GPU library on a certain system.
#define SWITCH_BACKEND_AF 0
#define SWITCH_BACKEND_CPU 1
#define SWITCH_BACKEND Z64C_SWITCH_BACKEND_CPU

/// Turn this on to ship the compiler with all features necessary to train its
/// models with unsupervised algorithms.
#define SWITCH_TRAIN 0

#define SWITCH_PORT 34290

/// Turn this on to allow engine clustering and networking capabilities. Not
/// supported yet.
#define SWITCH_CLUSTER 0

/// Place an upper limit to 'max_cache_size'.
#define SWITCH_MAX_CACHE_SIZE 1E+9

/// Change the default hashing function.
#define SWITCH_HASH_XXH64 0
#define SWITCH_HASH Z64C_SWITCH_HASH_XXH64

/// Size of contigouos memory chunks in the transpositions table.
#define SWITCH_TTABLE_FRAGMENT_SIZE (32 * 1024 * 1024)
