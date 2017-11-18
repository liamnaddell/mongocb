all:
	gcc -Wall -o mon main.c $(shell pkg-config --libs --cflags libmongoc-1.0) 
