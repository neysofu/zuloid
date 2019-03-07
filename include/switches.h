/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_SWITCHES_H
#define Z64C_SWITCHES_H

enum
{
	BACKEND_CPU,
	BACKEND_OPENCL,
};

#define SWITCH_BACKEND BACKEND_CPU
#define SWITCH_LOGGING
#define SWITCH_INIT_IMMEDIATELY
#define SWITCH_OPENMP

#endif
