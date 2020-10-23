#include "Main.h"
#include "fill_zero.h"
void fill_zero(int horizontal_map[VALUE][VALUE-1], int vertical_map[VALUE-1][VALUE], int player_map[VALUE-1][VALUE-1]) {
	int i, j;
	for (i = 0;i<VALUE;i++) {
		for (j = 0;j<VALUE-1;j++) {
			horizontal_map[i][j] = 0;
			vertical_map[j][i] = 0;
		}
	}
	for (i = 0;i<VALUE-1;i++) {
		for (j = 0;j<VALUE-1;j++) {
			player_map[i][j] = 0;
		}
	}
}