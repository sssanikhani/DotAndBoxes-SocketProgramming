#include "Main.h"
#include "down_check.h"
int down_check(int horizontal_map[VALUE][VALUE-1], int vertical_map[VALUE-1][VALUE], int player_map[VALUE-1][VALUE-1], int turn, int row, int column) {
	if (vertical_map[row - 1][column - 1] == 1 && vertical_map[row - 1][column] == 1 && horizontal_map[row][column - 1] == 1) {
		player_map[row - 1][column - 1] = turn + 1; /* for recognition of a cell completer based on player_map array, we use numbers(1,2) not numbers(0,1); because num0 in player_map array means that a cell is uncompleted. so, we used Turn+1 for filling player_map array in every completing cells*/
		return 1;
	}
	return 0;
}
