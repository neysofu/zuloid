/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Compile-time flags and switches. */

#pragma once

/* Activates debug print statements and assertions. Performance may drop when
 * turned on. */
#define SWITCH_DEBUG 1

/* Turns on multithreading capabilities via OpenMP. */
#define SWITCH_OPENMP 0

/// To choose the most performing GPU library on a certain system.
#define SWITCH_BACKEND_AF 0
#define SWITCH_BACKEND_CPU 1
#define SWITCH_BACKEND Z64C_SWITCH_BACKEND_CPU

/* Size of contigouos memory chunks in the transpositions table. */
#define SWITCH_TTABLE_BLOCK_SIZE (64 * 1024 * 1024)
