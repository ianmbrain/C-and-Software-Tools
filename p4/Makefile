CC = gcc
CFLAGS = -Wall -std=c99 -g -c -D_GNU_SOURCE

parks: parks.o catalog.o input.o
	gcc parks.o catalog.o input.o -o parks -lm

parks.o: parks.c catalog.h input.h
catalog.o: catalog.c catalog.h input.h
input.o: input.c input.h

clean:
	rm -f *.o
	rm -f parks
	rm -f output.txt
