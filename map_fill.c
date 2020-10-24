#include "map_fill.h"


void map_fill(const int map_size, 
				int horizontal_map[map_size][map_size - 1], 
					int vertical_map[map_size - 1][map_size], 
						int direction, int row, int column) {
	if (direction == 0) {
		horizontal_map[row - 1][column - 1] = 1;
	}
	else {
		vertical_map[row - 1][column - 1] = 1;
	}
}