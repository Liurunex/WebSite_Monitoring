CC=gcc
CFLAGS= -W -Wall
#CFLAGS= -W -Wall -g
LDFLAGS=
#LDFLAGS= -g
PROGS= tldmonitorLL tldmonitor

all: $(PROGS)

tldmonitorLL: tldmonitor.o date.o tldlistLL.o
	cc $(LDFLAGS) -o tldmonitorLL tldmonitor.o date.o tldlistLL.o

tldmonitor: tldmonitor.o date.o tldlist.o
	cc $(LDFLAGS) -o tldmonitor tldmonitor.o date.o tldlist.o

tldmonitor.o: tldmonitor.c date.h tldlist.h
date.o: date.h date.c
tldlist.o: tldlist.c date.h tldlist.h

clean:
	rm -f *~ *.o $(PROGS)
