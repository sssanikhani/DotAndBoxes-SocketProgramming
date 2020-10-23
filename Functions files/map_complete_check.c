#include "Main.h"
#include "map_complete_check.h"
int map_complete_check(int horizontal_map[VALUE][VALUE-1], int vertical_map[VALUE-1][VALUE]) {
	int i, j;
	for (i = 0;i<VALUE;i++) {
		for (j = 0;j<VALUE-1;j++) {
			if (horizontal_map[i][j] == 0) { 
				return 0;
			}
			if (vertical_map[j][i] == 0) {
				return 0;
			}
		}
	}
	return 1;
}