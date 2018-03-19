CC = gcc

dim: main.c misc.c
	$(CC) main.c -w -o dim -lncurses
