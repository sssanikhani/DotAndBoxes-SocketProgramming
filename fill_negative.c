#include "fill_negative.h"


void fill_negative(const int map_size, 
					int horizontal_map[map_size][map_size - 1], 
						int vertical_map[map_size - 1][map_size], 
							int player_map[map_size-1][map_size-1]) {
	int i, j;
	for (i = 0; i < map_size; i++) {
		for (j = 0; j < map_size - 1; j++) {
			horizontal_map[i][j] = -1;
			vertical_map[j][i] = -1;
		}
	}
	for (i = 0; i < map_size - 1; i++) {
		for (j = 0; j < map_size - 1; j++) {
			player_map[i][j] = -1;
		}
	}
}