#include "Main.h"
#include "map_print_VRT.h"
void map_print_VRT(int vertical_map[VALUE - 1][VALUE],int player_map[VALUE-1][VALUE-1], int row) {
	int i;
	for (i = 0;i<VALUE - 1;i++) {
		if (vertical_map[row][i] == 1) {
			printf("|");
		}
		else {
			printf(" ");
		}
		if (player_map[row][i] == 0) {
			printf(" ");
			printf(" ");
		}
		else if (player_map[row][i] == 1) {
			printf("A ");
		}
		else if (player_map[row][i] == 2) {
			printf("B ");
		}
	}
	if (vertical_map[row][VALUE - 1] == 1) {
		printf("|\n");
	}
	else {
		printf(" \n");
	}
}