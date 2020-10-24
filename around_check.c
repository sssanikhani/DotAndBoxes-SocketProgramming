#include "around_check.h"


int down_check(const int map_size, 
				int horizontal_map[map_size][map_size - 1], 
					int vertical_map[map_size - 1][map_size], 
						int player_map[map_size - 1][map_size - 1], 
							int turn, int row, int column) {
	
	if (vertical_map[row - 1][column - 1] == 1 
			&& vertical_map[row - 1][column] == 1 
			&& horizontal_map[row][column - 1] == 1) {
		
		player_map[row - 1][column - 1] = turn;
		return 1;
		
	}
	return 0;
}


int up_check(const int map_size, 
				int horizontal_map[map_size][map_size - 1], 
					int vertical_map[map_size - 1][map_size], 
						int player_map[map_size - 1][map_size - 1], 
									int turn, int row, int column) {
	if (vertical_map[row - 2][column - 1] == 1 
			&& vertical_map[row - 2][column] == 1 
			&& horizontal_map[row - 2][column - 1] == 1) {
		
		player_map[row - 2][column - 1] = turn;
		return 1;
		
	}
	return 0;
}


int left_check(const int map_size, 
				int horizontal_map[map_size][map_size - 1], 
					int vertical_map[map_size - 1][map_size], 
						int player_map[map_size - 1][map_size-1], 
									int turn, int row, int column) {
	
	if (horizontal_map[row - 1][column - 2] == 1 
			&& horizontal_map[row][column - 2] == 1 
			&& vertical_map[row - 1][column - 2] == 1) {
		
		player_map[row - 1][column - 2] = turn;
		return 1;

	}
	return 0;
}


int right_check(const int map_size, 
					int horizontal_map[map_size][map_size - 1], 
						int vertical_map[map_size - 1][map_size], 
							int player_map[map_size - 1][map_size - 1], 
										int turn, int row, int column) {
	if (horizontal_map[row - 1][column - 1] == 1 
			&& horizontal_map[row][column - 1] == 1 
			&& vertical_map[row - 1][column] == 1) {
		
		player_map[row - 1][column - 1] = turn;
		return 1;
	}
	return 0;
}