CC = gcc
CCFLAGS = -g -O3 -Wall
DEPS = sorted-list.h
OBJECTS = main.o sorted-list.o

all: sl library

%.o: %.c $(DEPS)
	$(CC) $(CCFLAGS) -c -o $@ $<

sl: $(OBJECTS)
	$(CC) $(CCFLAGS) -o $@ $^

library: sorted-list.o
	ar -cvr libsl.a sorted-list.o

clean:
	rm *.o *.a sl
