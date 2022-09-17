CC=gcc
CFLAGS_SEQ= -O3 -Wall -Werror -o lab1-seq main.c -lm
# -ftree-parallelize-loops=K - тут стоит 4, потому что у меня 4 ядра
CFLAGS_PAR_1= -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=1 main.c -o lab1-par-1 -lm
CFLAGS_PAR_3= -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=3 main.c -o lab1-par-3 -lm
CFLAGS_PAR_4= -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=4 main.c -o lab1-par-4 -lm
CFLAGS_PAR_5= -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=5 main.c -o lab1-par-5 -lm

all: main.c
	$(CC) $(CFLAGS_SEQ)
	$(CC) $(CFLAGS_PAR_1)
	$(CC) $(CFLAGS_PAR_3)
	$(CC) $(CFLAGS_PAR_4)
	$(CC) $(CFLAGS_PAR_5)

clean:
	rm -f main lab1-*