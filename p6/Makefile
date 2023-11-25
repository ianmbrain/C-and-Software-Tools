CC = gcc
CFLAGS += -Wall -std=c99 -g
LDLIBS += -o

driver: driver.o input.o value.o map.o
	gcc driver.o input.o value.o map.o -o driver

stringTest: stringTest.o value.o
	gcc stringTest.o value.o -o stringTest

mapTest: mapTest.o value.o map.o
	gcc mapTest.o value.o map.o -o mapTest

mapTest: mapTest.c value.h map.h
stringTest.o: stringTest.c value.h
driver.o: driver.c input.h value.h map.h
map.o: map.c map.h value.h
value.o: value.c value.h
input.o: input.c input.h

clean:
	rm -f *.o
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.gcov
	rm -f driver
	rm -f stringTest
	rm -f mapTest
	rm -f output.txt
