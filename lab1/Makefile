CC=gcc
SS=solarisstudio
ICC=icc

CFLAGS_SEQ= -O3 -Wall -Werror -o lab1-seq lab1.c -lm

# -ftree-parallelize-loops=K - тут стоит 4, потому что у меня 4 ядра
CFLAGS_PAR_1= -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=1 lab1.c -o lab1-par-1 -lm
CFLAGS_PAR_3= -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=3 lab1.c -o lab1-par-3 -lm
CFLAGS_PAR_4= -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=4 lab1.c -o lab1-par-4 -lm
CFLAGS_PAR_5= -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=5 lab1.c -o lab1-par-5 -lm

SS_FLAGS= -cc -O3 -xautopar -xloopinfo lab1.c

# -par-num-threads=K
ICC_FLAGS_1= -parallel -par-threshold0 -par-num-threads=1 -o lab1-icc-par-1 lab1.c
ICC_FLAGS_3= -parallel -par-threshold0 -par-num-threads=3 -o lab1-icc-par-3 lab1.c
ICC_FLAGS_4= -parallel -par-threshold0 -par-num-threads=4 -o lab1-icc-par-4 lab1.c
ICC_FLAGS_5= -parallel -par-threshold0 -par-num-threads=5 -o lab1-icc-par-5 lab1.c

all: seq par solaris intel

seq: lab1.c
	$(CC) $(CFLAGS_SEQ)

par: lab1.c
	$(CC) $(CFLAGS_PAR_1)
	$(CC) $(CFLAGS_PAR_3)
	$(CC) $(CFLAGS_PAR_4)
	$(CC) $(CFLAGS_PAR_5)

solaris: lab1.c
	$(SS) $(SS_FLAGS)

intel: lab1.c
	$(ICC) $(ICC_FLAGS_1)
	$(ICC) $(ICC_FLAGS_3)
	$(ICC) $(ICC_FLAGS_4)
	$(ICC) $(ICC_FLAGS_5)

clean:
	rm -f lab1 lab1-*