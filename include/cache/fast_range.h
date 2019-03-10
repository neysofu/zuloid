#ifndef Z64C_FAST_RANGE_H
#define Z64C_FAST_RANGE_H

#include <stdint.h>
#include <stdlib.h>

size_t
fast_range_32(uint_fast32_t x, size_t range);

size_t
fast_range_64(uint_fast64_t x, size_t range);

#endif
