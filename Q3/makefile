all: mync ttt

mync: mync.c
	gcc -Wall -g -o mync mync.c

ttt: ttt.c
	gcc -Wall -g -o ttt ttt.c

clean:
	rm -f ttt mync *.o

.PHONY: all clean