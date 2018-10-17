/**
 * @file scontroller.h
 * @brief Search controllers for high-level search operations.
 */

#pragma once

#include "ttable.h"
#include <stdint.h>

struct SController;

/**
 * @brief Create a new search controller based on the transpositions from @p
 * ttable.
 */
struct SController *
scontroller_new(struct TTable *ttable);

/** @brief Free @p scontroller and run destructors.
 */
void
scontroller_free(struct SController *scontroller);

Move
scontroller_best_move(struct SController *scontroller);

/**
 * @brief Stop searching.
 * @return The best move found.
 */
Move
scontroller_finish(struct SController *scontroller);
