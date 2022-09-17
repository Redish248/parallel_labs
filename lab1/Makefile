CC=gcc
CFLAGS_SEQ= -O3 -Wall -Werror -o lab1-seq main.c -lm
# -ftree-parallelize-loops=K - тут стоит 4, потому что у меня 4 ядра
CFLAGS_PAR= -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=4 main.c -o lab1-par-k -lm

all: main.c
	$(CC) $(CFLAGS_SEQ)
	$(CC) $(CFLAGS_PAR)
clean:
	rm -f main