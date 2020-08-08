CC = gcc
CFLAGS = -Wall -g

programa: programa.o vector.o

programa.o: programa.c vector.h

vector.o: vector.c vector.h

clean:
	rm -f programa *.o
