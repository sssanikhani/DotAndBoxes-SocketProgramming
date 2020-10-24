#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <bits/types.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "consts.h"
#include "fill_negative.h"
#include "map_complete_check.h"
#include "coordinate_check.h"
#include "get_score.h"
#include "map_fill.h"
#include "map_print.h"
#include "results_announcement.h"
#include "io_lib.h"





int next_turn(int pre_turn, int gp_size) {

	int turn = pre_turn;
	if (turn == gp_size - 1)
		turn = 0;
	else
		turn++;

	return turn;
	

}


int start_game(const int map_size, int id, int udp_sock, struct sockaddr_in *broadcast_addr) {

	int signal = 0;
	int horizontal_map[map_size][map_size - 1];
	int vertical_map[map_size - 1][map_size];
	int player_map[map_size-1][map_size-1];
	int direction, row, column, turn = 0, turn_score;
	int time_rem;
	int scores[map_size - MAPSIZE_DIFF_GPSIZE];
	for (int i = 0; i < map_size - MAPSIZE_DIFF_GPSIZE; i++) {
		scores[i] = 0;
	}
	
	char buffer[BUFF_SIZE];

	fd_set readfds;
	struct timeval tv;
	int activity;

	fill_negative(map_size, horizontal_map, vertical_map, player_map);
	
	while (map_complete_check(map_size, horizontal_map, vertical_map) == 0) {

		set_color(YELLOW);
		please_print("Turn: Player %d\n", turn);
		if (turn == id) {
			
			set_color(RESET);
			please_print("Please enter coordinates:\n");

			FD_ZERO(&readfds);
			FD_SET(STDIN_FILENO, &readfds);
			tv.tv_sec = USER_TURN_TIME;
			tv.tv_usec = 0;
			activity = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);
			if(activity == 0)
				signal = 1;

			
			if (FD_ISSET(STDIN_FILENO, &readfds)) {
				
				please_scan("%d %d %d", &direction, &row, &column);

			}
			
			if(signal == 0) {
				sprintf(buffer, "%d/(%d,%d,%d)", id, direction, row, column);
				if(sendto(udp_sock, buffer, strlen(buffer), 0, 
						(struct sockaddr *) broadcast_addr, sizeof(*broadcast_addr)) < 0) {

							set_color(RED);
							please_print("Error: Couldn't send coordinates\n");
							return -1;

				}
				recv(udp_sock, buffer, sizeof(buffer), 0);
				
				set_color(GREEN);
				please_print("Coordinates sent successfully!\n");
			}
			else {

				set_color(RED);
				please_print("Your turn time is finished!\n");

			}

		}
		else {

			int msg_turn;
			set_color(RESET);
			please_print("Waiting for player %d...\n", turn);

			FD_ZERO(&readfds);
			FD_SET(udp_sock, &readfds);
			tv.tv_sec = USER_TURN_TIME;
			tv.tv_usec = 0;
			activity = select(udp_sock + 1, &readfds, NULL, NULL, &tv);
			if(activity == 0)
				signal = 1;


			if(FD_ISSET(udp_sock, &readfds)) {

				int msg_len = recv(udp_sock, buffer, sizeof(buffer), 0);
				buffer[msg_len] = '\0';
				sscanf(buffer, "%d/(%d,%d,%d)", &msg_turn, &direction, &row, &column);

				if (msg_turn != turn) {

					set_color(RED);
					please_print("Error: A mismatch occured\n");
					return -1;

				}

				
			}
			else {

				set_color(RED);
				please_print("User %d turn time finished!\n", turn);

			}
			

		}

		if(signal == 0) {

			turn_score = get_score(map_size, horizontal_map, vertical_map,
									player_map, turn, direction, row, column);
			scores[turn] += turn_score;

			if (turn_score == 0) {

				turn = next_turn(turn, map_size - MAPSIZE_DIFF_GPSIZE);

			}

			map_fill(map_size, horizontal_map, vertical_map, direction, row, column);
			map_print(map_size, horizontal_map, vertical_map, player_map);

		}
		else {

			turn = next_turn(turn, map_size - MAPSIZE_DIFF_GPSIZE);

		}
		signal = 0;

		
	}
	results_announcement(map_size, scores);
	return 0;
}