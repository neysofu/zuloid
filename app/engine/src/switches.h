#pragma once

#include <stdbool.h>

// All commands starting with this character will be ignored. This is an
// extension to UCI that comes in handy for .ucirc, where comments might be
// useful.
#define Z64C_FLAG_COMMENT_CHAR '#'

// To choose the most performing GPU library on a certain system. Still WIP,
// only ArrayFire is supported at the moment.
#define Z64C_FLAG_BACKEND_AF 0
#define Z64C_FLAG_BACKEND Z64C_FLAG_BACKEND_AF

// Turn this on to ship the compiler with all features necessary to train its
// models with unsupervised algorithms.
#define Z64C_FLAG_TRAIN false

// Turn this on to allow engine clustering and networking capabilities. Not
// supported yet.
#define Z64C_FLAG_CLUSTER false

// Place an upper limit to 'max_cache_size_in_bytes'.
#define Z64C_FLAG_MAX_CACHE_SIZE_IN_BYTES 1+9
