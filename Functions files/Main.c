#include "Main.h"
#include "fill_zero.h"
#include "map_complete_check.h"
#include "coordinate_check.h"
#include "get_score.h"
#include "map_fill.h"
#include "map_print.h"
#include "results_announcement.h"

#include <sys/socket.h>
#include <unistd.h>


int main(const int map_size, int id, int udp_sock, struct sockaddr_in *broadcast_addr) {
	int horizontal_map[map_size][map_size - 1], vertical_map[map_size - 1][map_size], player_map[map_size-1][map_size-1], direction, row, column, turn = 0, a = 0, b = 0;
	fill_zero(horizontal_map, vertical_map, player_map); /* import 0 to all Arrays*/
	while (map_complete_check(horizontal_map, vertical_map) == 0) {
		if (turn == 0) {
			printf("Turn: Player A. Please enter coordinates:\n");
			scanf("%d %d %d", &direction, &row, &column);
		}
		else {
			printf("Turn: Player B. Please enter coordinates:\n");
			scanf("%d %d %d", &direction, &row, &column);
		}
		while (coordinate_check(horizontal_map, vertical_map, direction, row, column) == 0) {
			if (turn == 0) {
				printf("False coordinates!\nPlayer A! Please enter true coordinates:\n");
				scanf("%d %d %d", &direction, &row, &column);
			}
			else {
				printf("False coordinates!\nPlayer B! Please enter true coordinates:\n");
				scanf("%d %d %d", &direction, &row, &column);
			}
		}

		if (turn == 0) {
			a += get_score(horizontal_map, vertical_map,player_map,turn, direction, row, column);
			if (get_score(horizontal_map, vertical_map, player_map, turn, direction, row, column) == 0) {
				turn = 1;
			}
		}
		else {
			b += get_score(horizontal_map, vertical_map, player_map, turn, direction, row, column);
			if (get_score(horizontal_map, vertical_map, player_map, turn, direction, row, column) == 0) {
				turn = 0;
			}
		}
		map_fill(horizontal_map, vertical_map, direction, row, column); /* fill horizontal and vertical arrays based on gotten coordinates*/
		map_print(horizontal_map, vertical_map, player_map);
	}
	results_announcement(a, b);
	return 0;
}