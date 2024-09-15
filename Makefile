server: socket.c
	gcc socket.c -o server -lc -lpthread

postgres: postgres.c
	gcc postgres.c -o postgres -lpq
