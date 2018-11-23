/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once

enum Mode
{
	/* The engine is sitting idle and waiting for commands. */
	MODE_IDLE,
	/* The engine is currently busy searching. Incoming commands are still
	 * evaluated. */
	MODE_BUSY,
	/* The engine doesn't accept commands anymore and is ready to exit. */
	MODE_EXIT,
};
