#pragma once

struct Board *
tensor_to_board(struct TF_Tensor *tensor, struct Board *board);

struct TF_Tensor
board_to_tensor(struct Board *board, struct TF_Tensor *tensor);
