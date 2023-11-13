CC = gcc
CFLAGS = -Wall -std=c99 -g

all: encrypt decrypt

decrypt: decrypt.o io.o DES.o
	gcc decrypt.o io.o DES.o -o decrypt

encrypt: encrypt.o io.o DES.o
	gcc encrypt.o io.o DES.o -o encrypt

DESTest: DESTest.o DES.o DESMagic.o
	gcc DESTest.o DES.o DESMagic.o -o DESTest

DESTest.o: DESTest.c DES.h DESMagic.h
decrypt.o: decrypt.c io.h DES.h
encrypt.o: encrypt.c io.h DES.h
io.o: io.c io.h DES.h
DES.o: DES.c DES.h DESMagic.h
DESMagic.o: DESMagic.c DESMagic.h

clean:
	rm -f *.o
	rm -f encrypt
	rm -f decrypt
	rm -f DESTest
	rm -f output.txt
