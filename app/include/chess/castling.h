/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_CHESS_CASTLING_H
#define Z64C_CHESS_CASTLING_H

#include <stdint.h>

typedef uint_fast8_t CastlingRights;

enum
{
	CASTLING_RIGHT_W_OO = 0x1,
	CASTLING_RIGHT_B_OO = 0x2,
	CASTLING_RIGHT_W_OOO = 0x4,
	CASTLING_RIGHT_B_OOO = 0x8,
	CASTLING_RIGHTS_ALL = 0x15,
};

#endif
