#include "color.h"
#include "coord.h"

Rank
color_promoting_rank(const enum Color color) {
	switch (color) {
		case COLOR_WHITE:
			return 7;
		case COLOR_BLACK:
			return 0;
		default:
			return 0;
	}
}

Rank
color_pawn_rank(const enum Color color) {
	switch (color) {
		case COLOR_WHITE:
			return 1;
		case COLOR_BLACK:
			return 6;
		default:
			return 0;
	}
}

Rank
color_en_passant_rank(const enum Color color) {
	switch (color) {
		case COLOR_WHITE:
			return 2;
		case COLOR_BLACK:
			return 5;
		default:
			return 0;
	}
}

enum Color
color_other(enum Color color) {
	return !color;
}
