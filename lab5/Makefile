CC=gcc

CFLAGS= -O3 -Wall -Werror -o

all: lab5 static dynamic full verification

lab5: lab5.c
	$(CC) $(CFLAGS) lab5 lab5.c -lm -pthread -g

full: lab5_with_progress.c
	$(CC) $(CFLAGS) lab5p lab5_with_progress.c -lm -pthread -g
static: lab5_sample.c
	$(CC) $(CFLAGS) lab5s lab5_sample.c -lm -pthread -g
dynamic: lab5_dynamic.c
	$(CC) $(CFLAGS) lab5d lab5_dynamic.c -lm -pthread -g

verification: lab5_verification.c
	$(CC) $(CFLAGS) lab5v lab5_verification.c -lm -pthread -g

clean:
	rm -f lab5 lab5p lab5s
