CC=gcc

CFLAGS= -O3 -Wall -Werror -o lab3 lab3.c -lm -fopenmp
CFLAGS_SCHEDULE= -O3 -Wall -Werror -o schedule lab3_schedule.c -lm -fopenmp

all: lab3

lab3: lab3.c
	$(CC) $(LIB_LINK) $(CFLAGS) -g

scheduler: lab3_schedule.c
	$(CC) $(LIB_LINK) $(CFLAGS_SCHEDULE) -g

clean:
	rm -f lab3