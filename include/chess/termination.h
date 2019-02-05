/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_CHESS_TERMINATION_H
#define Z64C_CHESS_TERMINATION_H

/* See https://en.wikipedia.org/wiki/Rules_of_chess#End_of_the_game. */
enum Termination
{
	TERMINATION_CHECKMATE,
	TERMINATION_STALEMATE,
	TERMINATION_RESIGN,
	TERMINATION_TIME,
	TERMINATION_NO_CAPTURE,
	TERMINATION_REPETITION,
	TERMINATION_INSUFFICIENT_MATERIAL,
	TERMINATION_DRAW_BY_AGREEMENT,
	/* Crash, connectivity issues, illegal move... */
	TERMINATION_OTHER,
	/* An ongoing game. */
	TERMINATION_NONE,
};

#endif
