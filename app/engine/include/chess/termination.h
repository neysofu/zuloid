/// @file termination.h
/// @brief All ways in which a chess game can possibly end.

#pragma once

/// @see https://en.wikipedia.org/wiki/Rules_of_chess#End_of_the_game
enum Termination {
	TERMINATION_CHECKMATE,
	TERMINATION_STALEMATE,
	TERMINATION_RESIGN,
	TERMINATION_TIME,
	TERMINATION_NO_CAPTURE,
	TERMINATION_REPETITION,
	TERMINATION_INSUFFICIENT_MATERIAL,
	TERMINATION_DRAW_BY_AGREEMENT,
	// Crash, connectivity issues, illegal move...
	TERMINATION_OTHER,
	// An ongoing game has no termination.
	TERMINATION_NONE,
};
