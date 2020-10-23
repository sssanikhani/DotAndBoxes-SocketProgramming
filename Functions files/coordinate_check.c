#include "Main.h"
#include "coordinate_check.h"
int coordinate_check(int map_size, int horizontal_map[map_size][map_size - 1], int vertical_map[map_size - 1][map_size], int direction, int row, int column) {
	if (direction < 0 || direction > 1) { /* direction check*/
		return 0;
	}
	if (direction == 0) { /* row and column check */
		if (row < 1 || row > map_size || column < 1 || column > map_size - 1) {
			return 0;
		}
		if (horizontal_map[row - 1][column - 1] == 1) {
			return 0;
		}
	}
	else {
		if (row < 1 || row > map_size - 1 || column < 1 || column > map_size) {
			return 0;
		}
		if (vertical_map[row - 1][column - 1] == 1) {
			return 0;
		}
	}
	return 1;
}