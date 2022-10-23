CC=gcc

CFLAGS= -O3 -Wall -Werror -o lab4 lab4.c -lm -fopenmp

CFLAGS_MIN= -O3 -Wall -Werror -o lab4_task7_min lab4_task7_min.c -lm -fopenmp

CFLAGS_8= -O3 -Wall -Werror -o lab4_task8 lab4_task8.c -lm -fopenmp

CFLAGS_SCHEDULE_S1= -O3 -Wall -Werror -DSCHEDULE_TYPE=static -DCHUNK_SIZE=1 -o static1 lab4_schedule.c -lm -fopenmp
CFLAGS_SCHEDULE_S3= -O3 -Wall -Werror -DSCHEDULE_TYPE=static -DCHUNK_SIZE=3 -o static3 lab4_schedule.c -lm -fopenmp
CFLAGS_SCHEDULE_S4= -O3 -Wall -Werror -DSCHEDULE_TYPE=static -DCHUNK_SIZE=4 -o static4 lab4_schedule.c -lm -fopenmp
CFLAGS_SCHEDULE_S5= -O3 -Wall -Werror -DSCHEDULE_TYPE=static -DCHUNK_SIZE=5 -o static5 lab4_schedule.c -lm -fopenmp

CFLAGS_SCHEDULE_D1= -O3 -Wall -Werror -DSCHEDULE_TYPE=dynamic -DCHUNK_SIZE=1 -o dynamic1 lab4_schedule.c -lm -fopenmp
CFLAGS_SCHEDULE_D3= -O3 -Wall -Werror -DSCHEDULE_TYPE=dynamic -DCHUNK_SIZE=3 -o dynamic3 lab4_schedule.c -lm -fopenmp
CFLAGS_SCHEDULE_D4= -O3 -Wall -Werror -DSCHEDULE_TYPE=dynamic -DCHUNK_SIZE=4 -o dynamic4 lab4_schedule.c -lm -fopenmp
CFLAGS_SCHEDULE_D5= -O3 -Wall -Werror -DSCHEDULE_TYPE=dynamic -DCHUNK_SIZE=5 -o dynamic5 lab4_schedule.c -lm -fopenmp

CFLAGS_SCHEDULE_G1= -O3 -Wall -Werror -DSCHEDULE_TYPE=guided -DCHUNK_SIZE=1 -o guided1 lab4_schedule.c -lm -fopenmp
CFLAGS_SCHEDULE_G3= -O3 -Wall -Werror -DSCHEDULE_TYPE=guided -DCHUNK_SIZE=3 -o guided3 lab4_schedule.c -lm -fopenmp
CFLAGS_SCHEDULE_G4= -O3 -Wall -Werror -DSCHEDULE_TYPE=guided -DCHUNK_SIZE=4 -o guided4 lab4_schedule.c -lm -fopenmp
CFLAGS_SCHEDULE_G5= -O3 -Wall -Werror -DSCHEDULE_TYPE=guided -DCHUNK_SIZE=5 -o guided5 lab4_schedule.c -lm -fopenmp

CFLAGS0= -O0 -Wall -Werror -DSCHEDULE_TYPE=guided -DCHUNK_SIZE=5 -o lab4o0 lab4_schedule.c -lm -fopenmp
CFLAGS1= -O1 -Wall -Werror -DSCHEDULE_TYPE=guided -DCHUNK_SIZE=5 -o lab4o1 lab4_schedule.c -lm -fopenmp
CFLAGS2= -O2 -Wall -Werror -DSCHEDULE_TYPE=guided -DCHUNK_SIZE=5 -o lab4o2 lab4_schedule.c -lm -fopenmp
CFLAGS3= -O3 -Wall -Werror -DSCHEDULE_TYPE=guided -DCHUNK_SIZE=5 -o lab4o3 lab4_schedule.c -lm -fopenmp

all: lab4

schedule: static dynamic guided

lab4: lab4.c
	$(CC) $(CFLAGS) -g

lab4_min: lab4_task7_min.c
	$(CC) $(CFLAGS_MIN) -g

lab4_task8: lab4_task8.c
	$(CC) $(CFLAGS_8) -g

static: lab4_schedule.c
	$(CC) $(CFLAGS_SCHEDULE_S1) -g
	$(CC) $(CFLAGS_SCHEDULE_S3) -g
	$(CC) $(CFLAGS_SCHEDULE_S4) -g
	$(CC) $(CFLAGS_SCHEDULE_S5) -g

dynamic: lab4_schedule.c
	$(CC) $(CFLAGS_SCHEDULE_D1) -g
	$(CC) $(CFLAGS_SCHEDULE_D3) -g
	$(CC) $(CFLAGS_SCHEDULE_D4) -g
	$(CC) $(CFLAGS_SCHEDULE_D5) -g

guided: lab4_schedule.c
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
	rm -f lab4 static* dynamic* guided* lab4o* lab4_task7_min lab4_task8
