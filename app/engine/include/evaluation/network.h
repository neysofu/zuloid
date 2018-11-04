/**
 * @file network.h
 * @file Containes definitions for the evaluation function.
 */

#pragma once

#include <stdint.h>

struct Network;
struct BoardTensorIn;
struct BoardTensorOut;

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
             struct BoardTensor *bt,
             struct EvalTensor *et);
