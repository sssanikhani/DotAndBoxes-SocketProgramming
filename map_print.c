#include "consts.h"
#include "map_print.h"
#include "io_lib.h"


void map_print_VRT(const int map_size, 
					int vertical_map[map_size - 1][map_size],
						int player_map[map_size - 1][map_size - 1], int row) {
	set_color(YELLOW);
	int i;
	for (i = 0;i<map_size - 1;i++) {
		if (vertical_map[row][i] == 1) {
			please_print("|");
		}
		else {
			please_print(" ");
		}
		if (player_map[row][i] == -1) {
			please_print(" ");
			please_print(" ");
		}
		else{
			please_print("%d ", player_map[row][i]);
		}
	}
	if (vertical_map[row][map_size - 1] == 1) {
		please_print("|\n");
	}
	else {
		please_print(" \n");
	}
}


void map_print_HRZ(const int map_size, int horizontal_map[map_size][map_size - 1], int row) {
	set_color(YELLOW);
	int i;
	for (i = 0; i < map_size - 1; i++) {
		please_print("*");
		if (horizontal_map[row][i] == 1) {
			please_print("--");
		}
		else {
			please_print(" ");
			please_print(" ");
		}
	}
	please_print("*\n");
}


void map_print(const int map_size,
				int horizontal_map[map_size][map_size - 1], 
					int vertical_map[map_size - 1][map_size], 
						int player_map[map_size - 1][map_size - 1]) {
	int i, j;
	for (i = 0; i < map_size - 1; i++) {
		map_print_HRZ(map_size, horizontal_map, i);
		map_print_VRT(map_size, vertical_map, player_map, i);
	}
	map_print_HRZ(map_size, horizontal_map, map_size - 1);

	please_print("\n\n\n");
}