/// @file switches.h
/// @brief Compile-time flags.

#pragma once

#define SWITCH_DEBUG 1

/// To choose the most performing GPU library on a certain system.
#define SWITCH_BACKEND_AF 0
#define SWITCH_BACKEND_CPU 1
#define SWITCH_BACKEND Z64C_SWITCH_BACKEND_CPU

/// Turn this on to ship the compiler with all features necessary to train its
/// models with unsupervised algorithms.
#define SWITCH_TRAIN 0

/// Place an upper limit to 'max_cache_size'.
#define SWITCH_MAX_CACHE_SIZE 1E+9

/// Random search seed.
#define SWITCH_SEED 0xf9f868077a5940cf

/// Size of contigouos memory chunks in the transpositions table.
#define SWITCH_TTABLE_FRAGMENT_SIZE 16777216

#define SWITCH_NUM_AVG_CANDIDATE_MOVES 32
