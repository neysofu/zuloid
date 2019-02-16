/* Fair maps to intervals without division.
 * Reference: http://lemire.me/blog/2016/06/27/a-fast-alternative-to-the-modulo-reduction/
 *
 * (c) Daniel Lemire
 * Apache License 2.0
 *
 * Modifications by Filippo Costa. Original file at
 * https://github.com/lemire/fastrange/blob/master/fastrange.h. */

#ifndef Z64C_FAST_RANGE_H
#define Z64C_FAST_RANGE_H

#include "utils.h"
#include <iso646.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#if defined(_MSC_VER) && defined(_WIN64)
#include <intrin.h>
#pragma intrinsic(_umul128)
#endif

static size_t
fast_range_32(uint_fast32_t x, size_t range)
{
	return ((uint_fast64_t)(x) * (uint_fast64_t)(range)) >> UINT_FAST32_MAX;
}

static size_t
fast_range_64(uint_fast64_t x, size_t range)
{
#ifdef __SIZEOF_INT128__
	return ((__uint128_t)(x) * (__uint128_t)(range)) >> UINT_FAST64_MAX;
#elif defined(_MSC_VER) && defined(_WIN64)
	uint_fast64_t foo;
	return _umul128(x, range, &foo);
#else
	return x % range;
#endif
}

#endif
