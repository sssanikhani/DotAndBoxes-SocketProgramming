#include "Main.h"
#include "get_score.h"
#include "up_check.h"
#include "down_check.h"
#include "right_check.h"
#include "left_check.h"
int get_score(int horizontal_map[VALUE][VALUE-1], int vertical_map[VALUE-1][VALUE], int player_map[VALUE-1][VALUE-1], int turn, int direction, int row, int column) {
	int a = 0; 
	if (direction == 0) {
		if (row == 1) {
			if (down_check(horizontal_map, vertical_map, player_map, turn, row, column) == 1) {
				a++;
			}
		}
		else if (row == VALUE) {
			if (up_check(horizontal_map, vertical_map, player_map, turn, row, column) == 1) {
				a++;
			}
		}
		else {
			if (up_check(horizontal_map, vertical_map, player_map, turn, row, column) == 1) {
				a++;
			}
			if (down_check(horizontal_map, vertical_map, player_map, turn, row, column) == 1) {
				a++;
			}
		}
	}
	else {
		if (column == 1) {
			if (right_check(horizontal_map, vertical_map, player_map, turn, row, column) == 1) {
				a++;
			}
		}
		else if (column == VALUE) {
			if (left_check(horizontal_map, vertical_map, player_map, turn, row, column) == 1) {
				a++;
			}
		}
		else {
			if (right_check(horizontal_map, vertical_map, player_map, turn, row, column) == 1) {
				a++;
			}
			if (left_check(horizontal_map, vertical_map, player_map, turn, row, column) == 1) {
				a++;
			}
		}
	}
	return a; /* tedade cell haye por shode dar yek harekat*/
}