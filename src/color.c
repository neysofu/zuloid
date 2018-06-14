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

enum Color
color_other(enum Color color) {
	return !color;
}
