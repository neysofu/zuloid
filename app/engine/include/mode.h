/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @file mode.h
 * @brief All operational modes available.
 */

#pragma once

// Used as a status flag inside 'struct Driver' to keep track of internal
// state and to exit gracefully if needed.
enum Mode
{
	/// The engine is sitting idle and waiting for commands.
	MODE_IDLE,
	/// The engine is currently busy performing some calculations. Incoming
	/// commands will still be evaluated.
	MODE_BUSY,
	/// The engine is ready to exit.
	MODE_EXIT,
};
