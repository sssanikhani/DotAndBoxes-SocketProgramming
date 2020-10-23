#include "Main.h"
#include "map_print.h"
#include "map_print_HRZ.h"
#include "map_print_VRT.h"
void map_print(int horizontal_map[VALUE][VALUE-1], int vertical_map[VALUE-1][VALUE], int player_map[VALUE-1][VALUE-1]) {
	int i, j;
	for (i = 0;i<VALUE-1;i++) {
		map_print_HRZ(horizontal_map, i);
		map_print_VRT(vertical_map,player_map, i);
	}
	map_print_HRZ(horizontal_map, VALUE - 1);
}