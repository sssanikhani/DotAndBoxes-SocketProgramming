#include "Main.h"
#include "map_fill.h"
void map_fill(int horizontal_map[VALUE][VALUE-1], int vertical_map[VALUE-1][VALUE], int direction, int row, int column) {
	if (direction == 0) {
		horizontal_map[row - 1][column - 1] = 1;
	}
	else {
		vertical_map[row - 1][column - 1] = 1;
	}
}