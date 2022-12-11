CFLAGS=-std=c99 -g -pedantic-errors  -Wall -pedantic
all: main

clean:
	rm pa7

main: main.c log.c priorityQueue.c
		gcc $(CFLAGS) -o pa7 main.c log.c priorityQueue.c -lrt -L. -l:libruntime.so
