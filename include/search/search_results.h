/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_SEARCH_SEARCH_RESULTS_H
#define Z64C_SEARCH_SEARCH_RESULTS_H

struct SearchResults {
	double win_chances[2];
};

extern const struct SearchResults SEARCH_RESULTS_DEFAULT;

#endif
