CC=gcc

CFLAGS= -O3 -Wall -Werror -o lab5 lab5.c -lm -fopenmp

CFLAGS_SCHEDULE_S1= -O3 -Wall -Werror -DSCHEDULE_TYPE=static -DCHUNK_SIZE=1 -o static1 lab5_schedule.c -lm -fopenmp
CFLAGS_SCHEDULE_S3= -O3 -Wall -Werror -DSCHEDULE_TYPE=static -DCHUNK_SIZE=3 -o static3 lab5_schedule.c -lm -fopenmp
CFLAGS_SCHEDULE_S4= -O3 -Wall -Werror -DSCHEDULE_TYPE=static -DCHUNK_SIZE=4 -o static4 lab5_schedule.c -lm -fopenmp
CFLAGS_SCHEDULE_S5= -O3 -Wall -Werror -DSCHEDULE_TYPE=static -DCHUNK_SIZE=5 -o static5 lab5_schedule.c -lm -fopenmp

CFLAGS_SCHEDULE_D1= -O3 -Wall -Werror -DSCHEDULE_TYPE=dynamic -DCHUNK_SIZE=1 -o dynamic1 lab5_schedule.c -lm -fopenmp
CFLAGS_SCHEDULE_D3= -O3 -Wall -Werror -DSCHEDULE_TYPE=dynamic -DCHUNK_SIZE=3 -o dynamic3 lab5_schedule.c -lm -fopenmp
CFLAGS_SCHEDULE_D4= -O3 -Wall -Werror -DSCHEDULE_TYPE=dynamic -DCHUNK_SIZE=4 -o dynamic4 lab5_schedule.c -lm -fopenmp
CFLAGS_SCHEDULE_D5= -O3 -Wall -Werror -DSCHEDULE_TYPE=dynamic -DCHUNK_SIZE=5 -o dynamic5 lab5_schedule.c -lm -fopenmp

CFLAGS_SCHEDULE_G1= -O3 -Wall -Werror -DSCHEDULE_TYPE=guided -DCHUNK_SIZE=1 -o guided1 lab5_schedule.c -lm -fopenmp
CFLAGS_SCHEDULE_G3= -O3 -Wall -Werror -DSCHEDULE_TYPE=guided -DCHUNK_SIZE=3 -o guided3 lab5_schedule.c -lm -fopenmp
CFLAGS_SCHEDULE_G4= -O3 -Wall -Werror -DSCHEDULE_TYPE=guided -DCHUNK_SIZE=4 -o guided4 lab5_schedule.c -lm -fopenmp
CFLAGS_SCHEDULE_G5= -O3 -Wall -Werror -DSCHEDULE_TYPE=guided -DCHUNK_SIZE=5 -o guided5 lab5_schedule.c -lm -fopenmp

all: lab5 schedule

schedule: static dynamic guided

lab5: lab5.c
	$(CC) $(CFLAGS) -g

static: lab5_schedule.c
	$(CC) $(CFLAGS_SCHEDULE_S1) -g
	$(CC) $(CFLAGS_SCHEDULE_S3) -g
	$(CC) $(CFLAGS_SCHEDULE_S4) -g
	$(CC) $(CFLAGS_SCHEDULE_S5) -g

dynamic: lab5_schedule.c
	$(CC) $(CFLAGS_SCHEDULE_D1) -g
	$(CC) $(CFLAGS_SCHEDULE_D3) -g
	$(CC) $(CFLAGS_SCHEDULE_D4) -g
	$(CC) $(CFLAGS_SCHEDULE_D5) -g

guided: lab5_schedule.c
	$(CC) $(CFLAGS_SCHEDULE_G1) -g
	$(CC) $(CFLAGS_SCHEDULE_G3) -g
	$(CC) $(CFLAGS_SCHEDULE_G4) -g
	$(CC) $(CFLAGS_SCHEDULE_G5) -g

task14:
	$(CC) $(CFLAGS0) -g
	$(CC) $(CFLAGS1) -g
	$(CC) $(CFLAGS2) -g
	$(CC) $(CFLAGS3) -g

clean:
	rm -f lab5 static* dynamic* guided* 
