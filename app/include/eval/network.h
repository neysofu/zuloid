/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @file network.h
 * @file Containes definitions for the evaluation function.
 */

#pragma once

#include "chess/move.h"
#include <stdint.h>

struct Network;

struct Network *
network_new(void);

void
network_free(struct Network *network);

/**
 * @brief
 * @param network The neural network instance.
 * @param
 */
void
network_eval(struct Network *network,
             struct EvaluationHint *hint,
             struct Evaluation *eval_out,
			 Move move);
