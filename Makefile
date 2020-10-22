target:
	gcc -o server server.c
	gcc -o client client.c

	./server 8000

clean:
	rm server
	rm client