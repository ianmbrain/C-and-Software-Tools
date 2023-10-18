CC = gcc
CFLAGS = -Wall -std=c99 -g -c

trader: trader.o transaction.o account.o util.o
	gcc trader.o transaction.o account.o util.o -o trader

trader.o: trader.c transaction.h account.h
transaction.o: transaction.c transaction.h account.h util.h
account.o: account.c account.h util.h
util.o: util.c util.h

clean:
	rm -f *.o
	rm -f trader
	rm -f output.txt
