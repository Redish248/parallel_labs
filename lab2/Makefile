CC=gcc

LIN_LINK_N=-m64 -I/home/lab2/lib -L/home/lab2/lib
LIB_LINK= -m64 -I/home/redish/Documents/FW_1.3.1_Lin64 -L/home/redish/Documents/FW_1.3.1_Lin64/lib
CFLAGS_SEQ= -O3 -Wall -Werror -o lab2 lab2.c -lm -lfwSignal -lfwBase

all: lab2

lab2: lab2.c
	$(CC) $(LIB_LINK) $(CFLAGS_SEQ) -g
lab2n: lab2.c
	$(CC) $(LIB_LINK_N) $(CFLAGS_SEQ) -g

clean:
	rm -f lab2