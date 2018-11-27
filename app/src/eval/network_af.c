/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <stdlib.h>

#define NETWORK_CPU_WIDTH 128

/// 8192 KiB of weights in every layer.
//
// Inputs:
// * Turn (1).
// * Material imbalance (1).
// * Others...
// * Board tensor (64*64=4096).
// * Generic information (4096).
// Outputs of the network evaluation:
// * Valuea estimation, between 0 and 1.
// * Move values for knights (16*2=32).
// * Move values for bishops (13*3=39).
// * Move values for rooks (14*3=42).
// * Double pushes for pawns (8).
// * Single pushes for pawns (8).
// * Captures for pawns (8*2=16).
// We also use a novel architecture that is more scalable than before.
// The neural networks works by XNOR & POPCNT. We use integers, not floats,
// because floats waste many bits.
//
// So, the architecture is as following:
// * Entry tensor, each square has 256 bits for a total of 16384 bits.
// *
//
// Basically I need to create a mapping between first layer, hidden layers, and
// last layer. After that, I can just add hidden layers.
struct Network
{
};

struct TensorIn
{
};

struct TensorOut
{
};

int8_t
network_eval(struct Network *network,
             struct TensorIn *t_in,
             struct TensorOut *t_out)
{
	return 0;
}

void
network_train(struct Network *network, struct TensorIn *t_in)
{}
