int down_check(const int map_size, 
				int horizontal_map[map_size][map_size - 1], 
					int vertical_map[map_size - 1][map_size], 
						int player_map[map_size - 1][map_size - 1], 
							int turn, int row, int column);


int up_check(const int map_size, 
				int horizontal_map[map_size][map_size - 1], 
					int vertical_map[map_size - 1][map_size], 
						int player_map[map_size - 1][map_size - 1], 
									int turn, int row, int column);


int left_check(const int map_size, 
				int horizontal_map[map_size][map_size - 1], 
					int vertical_map[map_size - 1][map_size], 
						int player_map[map_size - 1][map_size-1], 
									int turn, int row, int column);


int right_check(const int map_size, 
					int horizontal_map[map_size][map_size - 1], 
						int vertical_map[map_size - 1][map_size], 
							int player_map[map_size - 1][map_size - 1], 
										int turn, int row, int column);