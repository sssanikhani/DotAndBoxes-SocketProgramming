void map_print_HRZ(const int map_size, int horizontal_map[map_size][map_size - 1], int row);


void map_print_VRT(const int map_size, 
					int vertical_map[map_size - 1][map_size],
						int player_map[map_size - 1][map_size - 1], int row);


void map_print(const int map_size,
				int horizontal_map[map_size][map_size - 1], 
					int vertical_map[map_size - 1][map_size], 
						int player_map[map_size - 1][map_size - 1]);