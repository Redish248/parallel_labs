CC=gcc

LIB_LINK= -m64 -c -I/home/redish/Documents/FW_1.3.1_Lin64/
CFLAGS_SEQ= -O3 -Wall -Werror -o lseq lab2.c -lm -lfwImage -lfwBase -lrt

# -ftree-parallelize-loops=K - тут стоит 4, потому что у меня 4 ядра
CFLAGS_PAR_1= -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=1 lab2.c -o lpar-1 -lm -lfwImage -lfwBase -lrt
CFLAGS_PAR_3= -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=3 lab2.c -o lpar-3 -lm -lfwImage -lfwBase -lrt
CFLAGS_PAR_4= -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=4 lab2.c -o lpar-4 -lm -lfwImage -lfwBase -lrt
CFLAGS_PAR_5= -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=5 lab2.c -o lpar-5 -lm -lfwImage -lfwBase -lrt


all: seq par

seq: lab2.c
	$(CC) -m64 -c -I/home/redish/Documents/FW_1.3.1_Lin64 lab2.c
	$(CC) -m64 -L/home/redish/Documents/FW_1.3.1_Lin64/lib lab2.o -lfwImage -lfwBase -lrt -lm

par: lab2.c
	$(CC) $(LIB_LINK) $(CFLAGS_PAR_1)
	$(CC) $(LIB_LINK) $(CFLAGS_PAR_3)
	$(CC) $(LIB_LINK) $(CFLAGS_PAR_4)
	$(CC) $(LIB_LINK) $(CFLAGS_PAR_5)


clean:
	rm -f lab1 lab1-* lsol* lpar-* lseq