CC = gcc
FLAGS = -Wall -g

.PHONY: all clean

all: ttt mync

ttt: ttt.c
	$(CC) $(FLAGS) -o $@ $^

mync: mync.c
	$(CC) $(FLAGS) -o $@ $^

clean:
	rm -f ttt mync *.o