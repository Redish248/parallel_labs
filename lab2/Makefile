CC=gcc

CFLAGS_SEQ= -O3 -Wall -Werror -o lseq lab1.c -lm

# -ftree-parallelize-loops=K - тут стоит 4, потому что у меня 4 ядра
CFLAGS_PAR_1= -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=1 lab1.c -o lpar-1 -lm
CFLAGS_PAR_3= -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=3 lab1.c -o lpar-3 -lm
CFLAGS_PAR_4= -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=4 lab1.c -o lpar-4 -lm
CFLAGS_PAR_5= -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=5 lab1.c -o lpar-5 -lm


all: seq par solaris intel

seq: lab1.c
	$(CC) $(CFLAGS_SEQ)

par: lab1.c
	$(CC) $(CFLAGS_PAR_1)
	$(CC) $(CFLAGS_PAR_3)
	$(CC) $(CFLAGS_PAR_4)
	$(CC) $(CFLAGS_PAR_5)


clean:
	rm -f lab1 lab1-* lsol* lpar-* lseq