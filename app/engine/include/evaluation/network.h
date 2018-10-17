#pragma once

#include <stdint.h>

struct Network;
struct TensorIn;
struct TensorOut;

struct Network *
network_new(void);

void
network_free(struct Network *network);

int8_t
network_eval(struct Network *network,
             struct TensorIn *t_in,
             struct TensorOut *t_out);
