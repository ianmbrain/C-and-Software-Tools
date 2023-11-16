CC = gcc
CFLAGS = -Wall -std=c99 -g

all: encrypt decrypt

decrypt: decrypt.o DES.o io.o DESMagic.o
	gcc decrypt.o io.o DES.o DESMagic.o -o decrypt

encrypt: encrypt.o DES.o io.o DESMagic.o
	gcc encrypt.o io.o DES.o DESMagic.o -o encrypt

DESTest: DESTest.o DES.o DESMagic.o
	gcc DESTest.o DES.o DESMagic.o -o DESTest

DESTest.o: DESTest.c DES.h DESMagic.h
decrypt.o: decrypt.c DES.h io.h
encrypt.o: encrypt.c DES.h io.h
io.o: io.c io.h DES.h
DES.o: DES.c DES.h DESMagic.h
DESMagic.o: DESMagic.c DESMagic.h

clean:
	rm -f *.o
	rm -f decrypt
	rm -f encrypt
	rm -f DESTest
	rm -f output.bin
	rm -f output.txt
	rm -f stderr.txt
	rm -f stdout.txt
