LIBS=-lpcap
CC=g++
CFLAGS=-Wall -g -pedantic

all: tcps

clean:
	rm -f tcps
	rm -f *.o

tcps: tcps.o tcpsdata.o
	$(CC) $(CFLAGS) -o $* tcps tcps.o tcpsdata.o $(LIBS)

tcps.o: tcps.cpp tcpsdata.o
	$(CC) $(CFLAGS) -c $*.cpp -o $@

tcpsdata.o: tcpsdata.cpp tcpsdata.h
	$(CC) $(CFLAGS) -c $*.cpp -o $@
