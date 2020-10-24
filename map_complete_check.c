#include "map_complete_check.h"


int map_complete_check(const int map_size, 
						int horizontal_map[map_size][map_size - 1], 
							int vertical_map[map_size - 1][map_size]) {
	
	int i, j;
	for (i = 0; i < map_size; i++) {
		for (j = 0; j < map_size - 1; j++) {
			if (horizontal_map[i][j] == -1) { 
				return 0;
			}
			if (vertical_map[j][i] == -1) {
				return 0;
			}
		}
	}
	return 1;

}