#include "board.h"

#define NETWORK_WIDTH 2

struct Model {
#ifdef SWITCH_BACKEND == SWITCH_BACKEND_CPU
	uint64_t layer_0[NETWORK_WIDTH * BOARD_NUM_SQUARES];
	uint64_t layer_1[NETWORK_WIDTH * BOARD_NUM_SQUARES];
	uint64_t layer_2[NETWORK_WIDTH * BOARD_NUM_SQUARES];
	uint64_t layer_3[NETWORK_WIDTH * BOARD_NUM_SQUARES];
	uint64_t layer_4[NETWORK_WIDTH * BOARD_NUM_SQUARES];
	uint64_t layer_5[NETWORK_WIDTH * BOARD_NUM_SQUARES];
	uint64_t layer_6[NETWORK_WIDTH * BOARD_NUM_SQUARES];
#elif SWITCH_BACKEND == SWITCH_BACKEND_AF
	af::array weights;
	af::array layer_1;
	af::array layer_2;
	af::array layer_3;
};

struct Model *
model_new(void) {
	model->layer_0(1, MODEL_WIDTH) = af::randu(1, MODEL_WIDTH);
	model->layer_1(1, MODEL_WIDTH) = af::randu(1, MODEL_WIDTH);
	model->layer_1(1, MODEL_WIDTH) = af::randu(1, MODEL_WIDTH);
	model->layer_1(1, MODEL_WIDTH) = af::randu(1, MODEL_WIDTH);
}

af::array
array_activate(af::array layer) {
	return layer * 2 / (layer ** 2 + 1);
}

Move
tensor_to_move(af::array tensor, Move move) {

}

#include <stdint.h>

uint64_t
model_compute(void) {
	return __builtin_popcount(0x1337);
}
