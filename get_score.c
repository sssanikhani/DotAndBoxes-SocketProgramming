#include "get_score.h"
#include "around_check.h"


int get_score(const int map_size, 
				int horizontal_map[map_size][map_size - 1], 
					int vertical_map[map_size - 1][map_size], 
						int player_map[map_size - 1][map_size - 1], 
							int turn, int direction, int row, int column) {
	int a = 0; 
	if (direction == 0) {
		if (row == 1) {
			if (down_check(map_size, horizontal_map, vertical_map, 
								player_map, turn, row, column) == 1) {
				a++;
			}
		}
		else if (row == map_size) {
			if (up_check(map_size, horizontal_map, vertical_map, 
								player_map, turn, row, column) == 1) {
				a++;
			}
		}
		else {
			if (up_check(map_size, horizontal_map, vertical_map, 
								player_map, turn, row, column) == 1) {
				a++;
			}
			if (down_check(map_size, horizontal_map, vertical_map, 
								player_map, turn, row, column) == 1) {
				a++;
			}
		}
	}
	else {
		if (column == 1) {
			if (right_check(map_size, horizontal_map, vertical_map, 
								player_map, turn, row, column) == 1) {
				a++;
			}
		}
		else if (column == map_size) {
			if (left_check(map_size, horizontal_map, vertical_map, 
								player_map, turn, row, column) == 1) {
				a++;
			}
		}
		else {
			if (right_check(map_size, horizontal_map, vertical_map, 
									player_map, turn, row, column) == 1) {
				a++;
			}
			if (left_check(map_size, horizontal_map, vertical_map, 
								player_map, turn, row, column) == 1) {
				a++;
			}
		}
	}
	return a;
}