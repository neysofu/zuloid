#include <stdbool.h>
#include "color.h"
#include "coordinates.h"

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
