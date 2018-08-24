#pragma once

#include "move.h"

#ifdef __cplusplus
extern "C" {
#endif

struct MoveStack;

struct MoveStack *
move_stack_new(void);

void
move_stack_drop(struct MoveStack *move_stack);

void
move_stack_push(struct MoveStack *move_stack, Move move);

size_t
move_stack_size(struct MoveStack *move_stack);

struct Move
move_stack_nth(struct MoveStack *move_stack);

#ifdef __cplusplus
};
#endif
