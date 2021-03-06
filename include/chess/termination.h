/* SPDX-License-Identifier: GPL-3.0-only */

#ifndef ZULOID_CHESS_TERMINATION_H
#define ZULOID_CHESS_TERMINATION_H

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
