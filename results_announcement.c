#include "consts.h"
#include "results_announcement.h"
#include "io_lib.h"


void results_announcement(const int map_size, int scores[map_size - MAPSIZE_DIFF_GPSIZE]) {

	int max = scores[0];
	int ind_max = 0;


	set_color(YELLOW);
	for (int i = 0; i < map_size - MAPSIZE_DIFF_GPSIZE; i++) {

		if (scores[i] > max) {
			max = scores[i];
			ind_max = i;
		}
		please_print("Player %d score: %d\n", i, scores[i]);

	}

	set_color(BLUE);
	please_print("Players with id < ");

	for (int i = 0; i < map_size - MAPSIZE_DIFF_GPSIZE; i++) {

		if(scores[i] == max)
		please_print("%d ", i);

	}

	please_print("> won the game with score %d!\n", max);
}