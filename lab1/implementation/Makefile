# Makefile, ECE254 lab1
# Irene Huang, 2012/09/20

CC=gcc
CFLAGS=-Wall -g
LD=gcc
LDFLAGS=-g 
LDLIBS=-lrt


OBJS1=sender.o point.o
OBJS2=receiver.o point.o

all: sender.out receiver.out

sender.out: $(OBJS1) 
	$(LD) $(LDFLAGS) -o $@ $(OBJS1) $(LDLIBS) 

sender.o: sender.c common.h point.h 
	$(CC) $(CFLAGS) -c sender.c 

receiver.out: $(OBJS2) 
	$(LD) $(LDFLAGS) -o $@ $(OBJS2) $(LDLIBS)

receiver.o: receiver.c common.h point.h 
	$(CC) $(CFLAGS) -c receiver.c 

point.o: point.c point.h 
	$(CC) $(CFLAGS) -c point.c 


.PHONY: clean
clean:
	rm -f *.o *.out 
