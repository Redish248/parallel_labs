CC=gcc
CFLAGS_SEQ= -O3 -Wall -Werror -o lab1-seq main.c
# lab1-par-K - тут стоит 4, потому что у меня 4 ядра
CFLAGS_PAR= -O3 -Wall -Werror -floopparallelize-all -ftree-parallelize-loops=K main.c -o lab1-par-4

main: main.c
	$(CC) $(CFLAGS_SEQ)
	$(CC) $(CFLAGS_PAR)
clean:
	rm -f main