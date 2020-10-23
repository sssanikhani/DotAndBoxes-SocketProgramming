#include "Main.h"
#include "map_print_HRZ.h"
void map_print_HRZ(int horizontal_map[VALUE][VALUE - 1], int row) {
	int i;
	for (i = 0;i<VALUE - 1;i++) {
		printf("*");
		if (horizontal_map[row][i] == 1) {
			printf("--");
		}
		else {
			printf(" ");
			printf(" ");
		}
	}
	printf("*\n");
}