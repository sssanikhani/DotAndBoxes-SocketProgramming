#include "results_announcement.h"
void results_announcement(int a, int b) {
	printf("Player A score: %d\n", a);
	printf("Player B score: %d\n", b);
	if (a > b) {
		printf("Player A won the game!\n");
	}
	else if(b > a) {
		printf("Player B won the game!\n");
	}
	else {
		printf("Draw!\n");
	}
}