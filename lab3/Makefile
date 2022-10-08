CC=gcc

CFLAGS= -O3 -Wall -Werror -o lab3 lab3.c -lm -fopenmp
CFLAGS_SCHEDULE_S1= -O3 -Wall -Werror -DSCHEDULE_TYPE=static -DCHUNK_SIZE=1 -o static1 lab3_schedule.c -lm -fopenmp
CFLAGS_SCHEDULE_S3= -O3 -Wall -Werror -DSCHEDULE_TYPE=static -DCHUNK_SIZE=3 -o static3 lab3_schedule.c -lm -fopenmp
CFLAGS_SCHEDULE_S4= -O3 -Wall -Werror -DSCHEDULE_TYPE=static -DCHUNK_SIZE=4 -o static4 lab3_schedule.c -lm -fopenmp
CFLAGS_SCHEDULE_S5= -O3 -Wall -Werror -DSCHEDULE_TYPE=static -DCHUNK_SIZE=5 -o static5 lab3_schedule.c -lm -fopenmp

CFLAGS_SCHEDULE_D1= -O3 -Wall -Werror -DSCHEDULE_TYPE=dynamic -DCHUNK_SIZE=1 -o dynamic1 lab3_schedule.c -lm -fopenmp
CFLAGS_SCHEDULE_D3= -O3 -Wall -Werror -DSCHEDULE_TYPE=dynamic -DCHUNK_SIZE=3 -o dynamic3 lab3_schedule.c -lm -fopenmp
CFLAGS_SCHEDULE_D4= -O3 -Wall -Werror -DSCHEDULE_TYPE=dynamic -DCHUNK_SIZE=4 -o dynamic4 lab3_schedule.c -lm -fopenmp
CFLAGS_SCHEDULE_D5= -O3 -Wall -Werror -DSCHEDULE_TYPE=dynamic -DCHUNK_SIZE=5 -o dynamic5 lab3_schedule.c -lm -fopenmp

CFLAGS_SCHEDULE_G1= -O3 -Wall -Werror -DSCHEDULE_TYPE=guided -DCHUNK_SIZE=1 -o guided1 lab3_schedule.c -lm -fopenmp
CFLAGS_SCHEDULE_G3= -O3 -Wall -Werror -DSCHEDULE_TYPE=guided -DCHUNK_SIZE=3 -o guided3 lab3_schedule.c -lm -fopenmp
CFLAGS_SCHEDULE_G4= -O3 -Wall -Werror -DSCHEDULE_TYPE=guided -DCHUNK_SIZE=4 -o guided4 lab3_schedule.c -lm -fopenmp
CFLAGS_SCHEDULE_G5= -O3 -Wall -Werror -DSCHEDULE_TYPE=guided -DCHUNK_SIZE=5 -o guided5 lab3_schedule.c -lm -fopenmp

CFLAGS1= -O1 -Wall -Werror -DSCHEDULE_TYPE=guided -DCHUNK_SIZE=5 -o lab3o1 lab3_schedule.c -lm -fopenmp
CFLAGS2= -O2 -Wall -Werror -DSCHEDULE_TYPE=guided -DCHUNK_SIZE=5 -o lab3o2 lab3_schedule.c -lm -fopenmp
CFLAGS4= -O4 -Wall -Werror -DSCHEDULE_TYPE=guided -DCHUNK_SIZE=5 -o lab3o4 lab3_schedule.c -lm -fopenmp

all: lab3

lab3: lab3.c
	$(CC) $(CFLAGS) -g

static: lab3_schedule.c
	$(CC) $(CFLAGS_SCHEDULE_S1) -g
	$(CC) $(CFLAGS_SCHEDULE_S3) -g
	$(CC) $(CFLAGS_SCHEDULE_S4) -g
	$(CC) $(CFLAGS_SCHEDULE_S5) -g

dynamic: lab3_schedule.c
	$(CC) $(CFLAGS_SCHEDULE_D1) -g
	$(CC) $(CFLAGS_SCHEDULE_D3) -g
	$(CC) $(CFLAGS_SCHEDULE_D4) -g
	$(CC) $(CFLAGS_SCHEDULE_D5) -g

guided: lab3_schedule.c
	$(CC) $(CFLAGS_SCHEDULE_G1) -g
	$(CC) $(CFLAGS_SCHEDULE_G3) -g
	$(CC) $(CFLAGS_SCHEDULE_G4) -g
	$(CC) $(CFLAGS_SCHEDULE_G5) -g

task14:
	$(CC) $(CFLAGS1) -g
	$(CC) $(CFLAGS2) -g
	$(CC) $(CFLAGS4) -g

clean:
	rm -f lab3 static* dynamic* guided* lab3o*