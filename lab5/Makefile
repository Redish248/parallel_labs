CC=gcc

CFLAGS= -O3 -Wall -Werror -o

all: lab5 static dynamic

lab5: lab5.c
	$(CC) $(CFLAGS) lab5 lab5.c -lm -pthread -g

static: lab5_with_progress.c
	$(CC) $(CFLAGS) lab5_p lab5_with_progress.c -lm -pthread -g

dynamic: lab5_dynamic.c
	$(CC) $(CFLAGS) dynamic lab5_dynamic.c -lm -pthread -g

clean:
	rm -f lab5 lab5_p
