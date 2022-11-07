#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <sys/types.h>

const int A = 936;
pthread_mutex_t print_mutex, chunk_mutex;
double *m1, *m2, *m2_copy;
int N, FOR_I, THREAD_NUM, start_chunk_size;

//Барьер - глобальная переменная
static pthread_barrier_t barrier;

struct main_args {
    int index;
    int id;
    int read;
    int write;
};

struct child_message {
    int msg_type;
};
struct master_message {
    int cosh_i;
    int fabs_i;
    int merge_i;
};

int cosh_index = 0;
int fabs_index = 0;
int merge_index = 0;

int get_cosh_index(int read_fd, int write_fd) {
    int r;
    struct child_message child;
    struct master_message master;
    write(write_fd, &child, sizeof(child));
    read(read_fd, &master, sizeof(master));

    return master.cosh_i;
}

int get_fabs_index(int read_fd, int write_fd) {
    int r;
    struct child_message child;
    struct master_message master;
    write(write_fd, &child, sizeof(child));
    read(read_fd, &master, sizeof(master));

    return master.fabs_i;
}

int get_merge_index(int read_fd, int write_fd) {
    int r;
    struct child_message child;
    struct master_message master;
    write(write_fd, &child, sizeof(child));
    read(read_fd, &master, sizeof(master));

    return master.merge_i;
}

/* comb_sort: function to find the new gap between the elements */
void comb_sort() { //
    double factor = 1.2473309; // фактор уменьшения
    long step = N / 2 - 1; // шаг сортировки

    while (step >= 1) {
        for (int i = 0; i + step < N / 2; i++) {
            if (m2[i] > m2[i + step]) {
                double tmp = m2[i];
                m2[i] = m2[i + step];
                m2[i + step] = tmp;
            }
        }
        step /= factor;
    }
}

void cosh_part(int max_size, int read_fd, int write_fd) {
    int start_i = get_cosh_index(read_fd, write_fd);

    while (start_i >= 0) {
        int len = start_chunk_size;
        if (start_i + len > max_size) len = max_size - start_i;

        int counter = 0;
        while (counter < len) {
            m1[start_i + counter] = cosh(m1[start_i + counter]) + 1;
            ++counter;
        }
        start_i = get_cosh_index(read_fd, write_fd);
    }
}

void fabs_part(int max_size, int read_fd, int write_fd) {
    int start_i = get_fabs_index(read_fd, write_fd);

    while (start_i >= 0) {
        int len = start_chunk_size;
        if (start_i + len > max_size) len = max_size - start_i;
        int counter = 0;
        while (counter < len) {
            int k = start_i + counter;
            if (k == 0) {
                m2[k] = fabs((double) 1 / tan(m2[k]));
            } else {
                m2[k] = fabs((double) 1 / tan(m2[k] + m2_copy[k - 1]));
            }
            ++counter;
        }
        start_i = get_fabs_index(read_fd, write_fd);
    }
}

void merge_part(int max_size, int read_fd, int write_fd) {
    int start_i = get_merge_index(read_fd, write_fd);

    while (start_i >= 0) {
        int len = start_chunk_size;
        if (start_i + len > max_size) len = max_size - start_i;
        int counter = 0;
        while (counter < len) {
            int k = start_i + counter;
            m2[k] = (double) m1[k] / m2[k];
            ++counter;
        }
        start_i = get_merge_index(read_fd, write_fd);
    }
}

void *main_function(void *args) {
    struct main_args thread_args = *((struct main_args *) args);

    int id = thread_args.id;
    unsigned int tmp1 = thread_args.index;
    unsigned int tmp2 = thread_args.index;
    int read_fd = thread_args.read, write_fd = thread_args.write;

    /*
    pthread_mutex_lock(&print_mutex);
    printf("thread %d start\n", id);
    pthread_mutex_unlock(&print_mutex);
     */

//    printf("#%d, start_i_m1=%d, delta=%d\n", id, start_i_1, chunk_size_1);
//    printf("#%d, start_i_m2=%d, delta=%d\n", id, start_i_2, chunk_size_2);

    unsigned int local_tmp1 = tmp1;
    unsigned int local_tmp2 = tmp2;

    // GENERATE

    if (id == 0) {
        for (int j = 0; j < N; j++) {
            double value = 1 + rand_r(&local_tmp1) % (A - 1);
            m1[j] = value;
//            printf("%2.f\n", value);
        }
        for (int j = 0; j < N / 2; j++) {
            double value = A + rand_r(&local_tmp2) % (A * 10 - A);
            m2[j] = value;
            m2_copy[j] = value;
//            printf("%.2f\n", value);
        }
    }

    /*
    pthread_mutex_lock(&print_mutex);
    printf("thread %d generate arr\n", id);
    pthread_mutex_unlock(&print_mutex);
    */
    pthread_barrier_wait(&barrier); // join потоков

    // MAP
    cosh_part(N, read_fd, write_fd);
    fabs_part(N / 2, read_fd, write_fd);

    pthread_barrier_wait(&barrier); // join потоков

    /*
    pthread_mutex_lock(&print_mutex);
    printf("thread %d map arr\n", id);
    if (id == 0) {
        printf("\n\nmap\n");
        for (int i = 0; i < N; i++) {
            printf("m1 #%d = %.2f\n", i, m1[i]);
        }
        for (int i = 0; i < N / 2; i++) {
            printf("m2 #%d = %2.f\n", i, m2[i]);
        }
    }
    pthread_mutex_unlock(&print_mutex);
    */

    // MERGE
    merge_part(N / 2, read_fd, write_fd);

    pthread_barrier_wait(&barrier); // join потоков

    /*
    pthread_mutex_lock(&print_mutex);
    if (id == 0) {
        printf("\n\nmerge\n");
        for (int i = 0; i < N / 2; i++) {
            printf("m2  #%d = %2.f\n", i, m2[i]);
        }
    }
    printf("thread %d merge arr\n", id);
    pthread_mutex_unlock(&print_mutex);
   */

    // SORT
    if (id == 0) {
        comb_sort();
        double result = 0;
        int j = 0;
        while (j < N / 2 && m2[j] == 0) {
            j++;
        }
        double min = m2[j];
        for (int k = 0; k < N / 2; k++) {
            if (((long) (m2[k] / min) % 2) == 0) {
                result += sin(m2[k]);
            }
        }

//        /*
        pthread_mutex_lock(&print_mutex);
        printf("X: %f\n", result);
        pthread_mutex_unlock(&print_mutex);
//         */
    }

    /*
    pthread_mutex_lock(&print_mutex);
    printf("thread %d finish\n", id);
    pthread_mutex_unlock(&print_mutex);
    */


    pthread_exit(NULL);
}

void process_messages() {
    char readbuffer[80];
    int finish = 1;
    while (finish > 0) {
        int n = read(readbuffer, sizeof(readbuffer));

    }
}

int main(int argc, char *argv[]) {
    struct timeval T1, T2;
    long delta_ms;

    if (argc < 4) {
        printf("Need to add size of array and number of threads as input arguments\n");
        return -1;
    }

    N = atoi(argv[1]);
    int M = atoi(argv[2]);
    if (M < 1) {
        printf("threads_counter must be positive");
        return -1;
    }

    start_chunk_size = atoi(argv[3]);
    if (start_chunk_size < 1 || start_chunk_size >= N) {
        printf("incorrect start_chuck_size parameter");
        return -2;
    }

    if (argc >= 5) {
        FOR_I = atoi(argv[4]);
    } else FOR_I = 100;

    THREAD_NUM = M;
    if (N < THREAD_NUM) THREAD_NUM = N;

    m1 = (double *) malloc(N * sizeof(double));
    m2 = (double *) malloc(N / 2 * sizeof(double));
    m2_copy = (double *) malloc(N / 2 * sizeof(double));

    pthread_mutex_init(&print_mutex, NULL);
    pthread_mutex_init(&chunk_mutex, NULL);
    pthread_barrier_init(&barrier, NULL, THREAD_NUM); //инициализация барьера
    pthread_t thread[THREAD_NUM];
    struct main_args thread_args[THREAD_NUM];

    gettimeofday(&T1, NULL);

    for (int l = 0; l < FOR_I; l++) {
        int cosh_i = 0;
        int fabs_i = 0;
        int merge_i = 0;

        int fds_read[THREAD_NUM];
        int fds_write[THREAD_NUM];

        for (int i = 0; i < THREAD_NUM; i++) { // 1, 2 ... THREAD_NUM-1
            int fd_child_to_master[2];
            int fd_master_to_child[2];
            pipe(fd_child_to_master);
            pipe(fd_master_to_child);

            thread_args[i].index = l;
            thread_args[i].id = i;
            thread_args[i].write = fd_child_to_master[0];
            thread_args[i].read = fd_master_to_child[1];

            fds_read[i] = fd_child_to_master[1];
            fds_write[i] = fd_master_to_child[0];

            //            printf("create thread #%d\n", i);

            pthread_create(&thread[i], NULL, main_function, &thread_args[i]);
            close(fd_child_to_master[1]);
            close(fd_master_to_child[0]);
        }
        //        printf("main create all\n");

        int finish = 0;
        int n;
        struct child_message readbuf;
        while (finish < THREAD_NUM) {
            for (int j = 0; j < THREAD_NUM; j++) {
                n = read(fds_read[j], &readbuf, sizeof(readbuf));
                if (readbuf.msg_type == 0) finish++;
                else {
                    int c = cosh_i >= N ? -1 : cosh_i;
                    int f = fabs_i >= N / 2 ? -1 : fabs_i;
                    int m = merge_i >= N / 2 ? -1 : fabs_i;

                    struct master_message msg = {c, f, m};
                    write(fds_write[j], &msg, sizeof(msg));
                };
            }
        }

        for (int i = 0; i < THREAD_NUM; i++) {
            pthread_join(thread[i], NULL);
        }
    }

    //Выход из потока:
    pthread_barrier_destroy(&barrier);

    gettimeofday(&T2, NULL);
    delta_ms = 1000 * (T2.tv_sec - T1.tv_sec) + (T2.tv_usec - T1.tv_usec) / 1000;
    printf("%d;%ld\n", N, delta_ms);
}

}
