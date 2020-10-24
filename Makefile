target:
	gcc -c around_check.c
	gcc -c fill_negative.c
	gcc -c game.c
	gcc -c get_score.c
	gcc -c io_lib.c
	gcc -c map_complete_check.c
	gcc -c map_fill.c
	gcc -c map_print.c
	gcc -c results_announcement.c
	gcc -c server.c
	gcc -c client.c

	gcc -o client client.o around_check.o fill_negative.o \
			game.o get_score.o io_lib.o map_complete_check.o map_fill.o map_print.o \
			results_announcement.o
	
	gcc -o server server.o io_lib.o


clean:
	rm -f server
	rm -f client
	rm -f server.o
	rm -f client.o
	rm -f around_check.o
	rm -f fill_negative.o
	rm -f game.o
	rm -f get_score.o
	rm -f io_lib.o
	rm -f map_complete_check.o
	rm -f map_fill.o
	rm -f map_print.o
	rm -f results_announcement.o