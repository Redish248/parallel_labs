CC=gcc
SS=suncc
ICC=icc

CFLAGS_SEQ= -O3 -Wall -Werror -o lseq lab1.c -lm

# -ftree-parallelize-loops=K - тут стоит 4, потому что у меня 4 ядра
CFLAGS_PAR_1= -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=1 lab1.c -o lpar-1 -lm
CFLAGS_PAR_3= -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=3 lab1.c -o lpar-3 -lm
CFLAGS_PAR_4= -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=4 lab1.c -o lpar-4 -lm
CFLAGS_PAR_5= -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=5 lab1.c -o lpar-5 -lm

#-xOK, K=уровень распараллеливания
SS_FLAGS_1= -xO1 -xautopar -xloopinfo -o lsol1 lab1.c -lm
SS_FLAGS_3= -xO3 -xautopar -xloopinfo -o lsol3 lab1.c -lm
SS_FLAGS_4= -xO4 -xautopar -xloopinfo -o lsol4 lab1.c -lm
SS_FLAGS_5= -xO5 -xautopar -xloopinfo -o lsol5 lab1.c -lm

# -par-num-threads=K
ICC_FLAGS_1= -parallel -par-threshold0 -par-num-threads=1 -o lpar-icc-1 lab1.c
ICC_FLAGS_3= -parallel -par-threshold0 -par-num-threads=3 -o lpar-icc-3 lab1.c
ICC_FLAGS_4= -parallel -par-threshold0 -par-num-threads=4 -o lpar-icc-4 lab1.c
ICC_FLAGS_5= -parallel -par-threshold0 -par-num-threads=5 -o lpar-icc-5 lab1.c

all: seq par solaris intel

seq: lab1.c
	$(CC) $(CFLAGS_SEQ)

par: lab1.c
	$(CC) $(CFLAGS_PAR_1)
	$(CC) $(CFLAGS_PAR_3)
	$(CC) $(CFLAGS_PAR_4)
	$(CC) $(CFLAGS_PAR_5)

solaris: lab1.c
	$(SS) $(SS_FLAGS_1)
	$(SS) $(SS_FLAGS_3)
	$(SS) $(SS_FLAGS_4)
	$(SS) $(SS_FLAGS_5)

intel: lab1.c
	$(ICC) $(ICC_FLAGS_1)
	$(ICC) $(ICC_FLAGS_3)
	$(ICC) $(ICC_FLAGS_4)
	$(ICC) $(ICC_FLAGS_5)

clean:
	rm -f lab1 lab1-* lsol* lpar-*