#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include <pthread.h>

int *percent;
const int A = 936;
pthread_mutex_t percent_mutex, print_mutex;
double *m1, *m2, *m2_copy;
int N, FOR_I, THREAD_NUM;

//Барьер - глобальная переменная
static pthread_barrier_t barrier;

struct main_args {
    int index;
    int id;
};

void print_arr(double *arr, int size) {
    printf("size = %d\n", size);
//    for (int i = 0; i < size; i++) {
    int i = 0;
    printf("%d => %f\n", i, arr[i]);
//    }
}

/* comb_sort: function to find the new gap between the elements */
void comb_sort() { //
    double factor = 1.2473309; // фактор уменьшения
    long step = N - 1; // шаг сортировки

    while (step >= 1) {
        for (int i = 0; i + step < N; i++) {
            if (m2[i] > m2[i + step]) {
                double tmp = m2[i];
                m2[i] = m2[i + step];
                m2[i + step] = tmp;
            }
        }
        step /= factor;
    }
}

void generate_part_m1(unsigned int tmp, int start_index, int len) {
    int counter = 0;
    while (counter < len) {
        double value = 1 + rand_r(&tmp) % (A - 1);
        m1[start_index + counter] = value;
        ++counter;
    }
}

void generate_part_m2(unsigned int tmp, int start_index, int len) {
    int counter = 0;
    while (counter < len) {
        double value = 1 + rand_r(&tmp) % (A - 1);
        m2[start_index + counter] = value;
        m2_copy[start_index + counter] = value;
        ++counter;
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

int count_len(int start_i, int size) {
    int delta = size / THREAD_NUM;
    int len;
    if (N - (start_i * delta) > 2 * delta) len = delta;
    else len = N - (start_i * delta);
    return len;
}

void *main_function(void *args) {
    struct main_args thread_args = *((struct main_args *) args);

    int id = thread_args.id;
    unsigned int tmp1 = thread_args.index;
    unsigned int tmp2 = thread_args.index;

//    /*
    pthread_mutex_lock(&print_mutex);
    printf("thread %d start\n", id);
    pthread_mutex_unlock(&print_mutex);
//     */

    int size_1 = N / THREAD_NUM;
    int size_2 = N / THREAD_NUM / 2;
    int delta_1 = size_1;
    int delta_2 = size_2;
    int start_i_1 = delta_1 * id;
    int start_i_2 = delta_2 * id;

    int len_1 = count_len(start_i_1, size_1);
    int len_2 = count_len(start_i_2, size_2);

    unsigned int local_tmp1 = tmp1;
    unsigned int local_tmp2 = tmp2;

    // GENERATE
    generate_part_m1(local_tmp1, start_i_1, len_1);
    generate_part_m2(local_tmp2, start_i_2, len_2);

//    /*
    pthread_mutex_lock(&print_mutex);
    printf("thread %d generate arr\n", id);
    pthread_mutex_unlock(&print_mutex);
//    */
    pthread_barrier_wait(&barrier); // join потоков

    // MAP
    cosh_part(start_i_1, len_1);
    fabs_part(start_i_2, len_2);

//    /*
    pthread_mutex_lock(&print_mutex);
    printf("thread %d map arr\n", id);
    pthread_mutex_unlock(&print_mutex);
//    */

    pthread_barrier_wait(&barrier); // join потоков

    // MERGE
    merge_part(start_i_2, len_2);

//    /*
    pthread_mutex_lock(&print_mutex);
    printf("thread %d merge arr\n", id);
    pthread_mutex_unlock(&print_mutex);
//    */

    pthread_barrier_wait(&barrier); // join потоков

    // SORT
    if (id == 1) {
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

        pthread_mutex_lock(&print_mutex);
        printf("X: %f\n", result);
        pthread_mutex_unlock(&print_mutex);
    }

//    /*
    pthread_mutex_lock(&print_mutex);
    printf("thread %d finish\n", id);
    pthread_mutex_unlock(&print_mutex);
//    */

    //Выход из потока:
    int status = pthread_barrier_wait(&barrier);
    if (status == PTHREAD_BARRIER_SERIAL_THREAD) {
        pthread_barrier_destroy(&barrier);
    } else if (status != 0) {
        exit(-10);
    }

    pthread_exit(NULL);
}

void *percent_counter() {
    int value;
    for (;;) {
        pthread_mutex_lock(&percent_mutex);
        value = *percent;
        printf("Current percent: %d\n", value);
        pthread_mutex_unlock(&percent_mutex);

        if (value >= 100) break;
        sleep(1);
    }
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
    THREAD_NUM = atoi(argv[2]);
    if (THREAD_NUM < 1) printf("threads_counter must be positive");
    if (argc >= 4) {
        FOR_I = atoi(argv[3]);
    } else FOR_I = 100;

    m2 = (double *) malloc(N * sizeof(double));
    m2 = (double *) malloc(N / 2 * sizeof(double));
    m2_copy = (double *) malloc(N / 2 * sizeof(double));

    percent = (int *) malloc(sizeof(int));
    *percent = 0;

    pthread_mutex_init(&percent_mutex, NULL);
    pthread_mutex_init(&print_mutex, NULL);
    pthread_barrier_init(&barrier, NULL, THREAD_NUM - 1); //инициализация барьера
    pthread_t thread[THREAD_NUM];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    struct main_args thread_args[THREAD_NUM - 1];

    gettimeofday(&T1, NULL);

    pthread_create(&thread[0], &attr, percent_counter, argv[1]);

    for (int l = 0; l < FOR_I; l++) {
        for (int i = 1; i < THREAD_NUM; i++) { // 1, 2 ... THREAD_NUM-1
            thread_args[i - 1].index = l ;
            thread_args[i - 1].id = i-1 ;

            printf("create thread #%d\n", i-1);
            pthread_t tid = thread[i];
            pthread_create(&tid, NULL, main_function, &thread_args[i-1]);
        }

        for (int i = 0; i < THREAD_NUM; i++) {
            pthread_join(thread[i], NULL);
        }

        pthread_mutex_lock(&percent_mutex);
        *percent = (100 * (l + 1)) / FOR_I;
        pthread_mutex_unlock(&percent_mutex);
    }

    gettimeofday(&T2, NULL);
    delta_ms = 1000 * (T2.tv_sec - T1.tv_sec) + (T2.tv_usec - T1.tv_usec) / 1000;
    printf("%d;%ld\n", N, delta_ms);

    free(percent);
    free(m1);
    free(m2);
    free(m2_copy);
}
