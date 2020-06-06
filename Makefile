CC = gcc
CFLAGS = -Wall -std=c99 -g

shopping: item.o list.o shopping.o

shopping.o: item.h list.h
item.o: item.h
list.o: item.h list.h

clean:
	rm -f shopping shopping.o
	rm -f item item.o
	rm -f list list.o