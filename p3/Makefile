CC = gcc
CFLAGS = -Wall -std=c99 -g -c

trader: trader.o transaction.o account.o
	gcc trader.o transaction.o account.o -o trader

trader.o: trader.c transaction.h account.h
transaction.o: transaction.c transaction.h account.h util.h
account.o: account.c account.h util.h
util.o: util.c util.h
	