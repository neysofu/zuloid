/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "eval/network.h"
#include "switches.h"
#include <stdlib.h>

struct Network {

};

struct Network *
network_new(void)
{
	return xmalloc(sizeof(struct Network));
}

void
network_free(struct Network *network)
{
	free(network);
}
