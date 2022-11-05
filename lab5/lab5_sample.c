#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include <pthread.h>

const int A = 936;
pthread_mutex_t print_mutex;
double *m1, *m2, *m2_copy;
int N, FOR_I, THREAD_NUM;

//Барьер - глобальная переменная
static pthread_barrier_t barrier;

struct main_args {
    int index;
    int id;

};

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

void cosh_part(int start_i, int len) {
    int counter = 0;
    while (counter < len) {
        m1[start_i + counter] = cosh(m1[start_i + counter]) + 1;
        ++counter;
    }
}

void fabs_part(int start_i, int len) {
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
}

void merge_part(int start_i, int len) {
    int counter = 0;
    while (counter < len) {
        int k = start_i + counter;
        m2[k] = (double) m1[k] / m2[k];
        ++counter;
    }
}

void *main_function(void *args) {
    struct main_args thread_args = *((struct main_args *) args);

    int id = thread_args.id;
    unsigned int tmp1 = thread_args.index;
    unsigned int tmp2 = thread_args.index;

    int chunk_size_1 = N / THREAD_NUM;
    int start_i_1 = id * chunk_size_1;
    if (id == THREAD_NUM - 1) { // last 
        chunk_size_1 = N - start_i_1;
    }

    int chunk_size_2 = N / THREAD_NUM / 2;
    int start_i_2 = id * chunk_size_2;
    if (id == THREAD_NUM - 1) { // last 
        chunk_size_2 = N - start_i_2;
    }

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
    if (chunk_size_1 > 0) cosh_part(start_i_1, chunk_size_1);
    if (chunk_size_2 > 0) fabs_part(start_i_2, chunk_size_2);

    /*
    pthread_mutex_lock(&print_mutex);
    printf("thread %d map arr\n", id);
    if (id == 0) {
        printf("\n\nmap\n");
        for (int i = 0; i < N; i++) {
            printf("m1 %.2f\n", m1[i]);
        }
        for (int i = 0; i < N / 2; i++) {
            printf("m2 %2.f\n ", m2[i]);
        }
    }
    pthread_mutex_unlock(&print_mutex);
    */

    pthread_barrier_wait(&barrier); // join потоков

    // MERGE
    if (chunk_size_2 > 0) merge_part(start_i_2, chunk_size_2);

    /*
    pthread_mutex_lock(&print_mutex);
    if (id == 0) {
        printf("\n\nmerge\n");
        for (int i = 0; i < N / 2; i++) {
            printf("m2 %2.f\n ", m2[i]);
        }
    }
    printf("thread %d merge arr\n", id);
    pthread_mutex_unlock(&print_mutex);
   */

    pthread_barrier_wait(&barrier); // join потоков

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

        /*
        pthread_mutex_lock(&print_mutex);
        printf("X: %f\n", result);
        pthread_mutex_unlock(&print_mutex);
         */
    }

    /*
    pthread_mutex_lock(&print_mutex);
    printf("thread %d finish\n", id);
    pthread_mutex_unlock(&print_mutex);
    */


    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    struct timeval T1, T2;
    long delta_ms;

    if (argc < 3) {
        printf("Need to add size of array and number of threads as input arguments\n");
        return -1;
    }

    N = atoi(argv[1]);
    int M = atoi(argv[2]);
    if (M < 1) printf("threads_counter must be positive");
    if (argc >= 4) {
        FOR_I = atoi(argv[3]);
    } else FOR_I = 100;

    THREAD_NUM = M;
    if (N < THREAD_NUM) THREAD_NUM = N;

    m1 = (double *) malloc(N * sizeof(double));
    m2 = (double *) malloc(N / 2 * sizeof(double));
    m2_copy = (double *) malloc(N / 2 * sizeof(double));

    pthread_mutex_init(&print_mutex, NULL);
    pthread_barrier_init(&barrier, NULL, THREAD_NUM); //инициализация барьера
    pthread_t thread[THREAD_NUM];
    struct main_args thread_args[THREAD_NUM];

    gettimeofday(&T1, NULL);

    for (int l = 0; l < FOR_I; l++) {
        for (int i = 0; i < THREAD_NUM; i++) { // 1, 2 ... THREAD_NUM-1
            thread_args[i].index = l;
            thread_args[i].id = i;

//            printf("create thread #%d\n", i);
            pthread_create(&thread[i], NULL, main_function, &thread_args[i]);
        }
//        printf("main create all\n");


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
