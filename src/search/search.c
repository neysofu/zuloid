/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chess/position.h"
#include "engine.h"
#include <stdint.h>

/* The search routine finds perfect play within a certain statistical
 * dispersion threshold. The greater the threshold, the deeper the search.
 * Assuming that evaluations behave like distributions, we can build a
 * theoretical framework to find the optimal algorithm.
 *
 * A. Point to a specific position.
 * B. Jump to step E if the position is in the cache and has a low
 *    dispersion score.
 * C. Evaluate the position and backtrace its evaluation to the top of the tree.
 * D. Flip its dispersion bit.
 * E. Point to the next position. Go back to step B.
 *
 * Positions are searched best-first.
 *
 * 1. Standard position.
 * 2. e4
 * 3. e4 b5
 * 4. e4 d5
 * 5. Nf3
 * Done. Only the most uncertiain positions have been searched. */
void
engine_search(struct Engine *engine, uint_fast32_t dispersion_threshold)
{
	/* The basic idea is: not only cache evaluations, but whole subtrees. This
	 * can be done only if the confidence level is good enough. Therefore, either
	 *  - It was searched in this round
	 *  - I have explicit confidence. */
	struct CacheEntry *cache_entry = cache_get(engine->cache, &engine->position);
	if (cache_entry) {
		return;
	} else if (cache_entry) {
		return;
	} else {
		return;
	}
}
