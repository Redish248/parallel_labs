#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>

#include <pthread.h>

int *percent;
const int A = 936;
pthread_mutex_t percent_mutex;
double *m1, *m2, *m2_copy;
int N, FOR_I, THREAD_NUM;

/* comb_sort: function to find the new gap between the elements */
void comb_sort(double data[], int size) { //
    double factor = 1.2473309; // фактор уменьшения
    long step = size - 1; // шаг сортировки

    while (step >= 1) {
        for (int i = 0; i + step < size; i++) {
            if (data[i] > data[i + step]) {
                double tmp = data[i];
                data[i] = data[i + step];
                data[i + step] = tmp;
            }
        }
        step /= factor;
    }
}

void join_section_arrays(double *res_array, const double *part1, int size1, const double *part2, int size2) {
    int i = 0, j = 0, i_res = 0;

    for (; i < size1 && j < size2;) {
        if (part1[i] < part2[j]) {
            res_array[i_res++] = part1[i++];
        } else {
            res_array[i_res++] = part2[j++];
        }
    }

    while (i < size1) {
        res_array[i_res++] = part1[i++];
    }
    while (j < size2) {
        res_array[i_res++] = part2[j++];
    }
}

void copy_result(const double *src, double *dst, int size) {
#pragma omp parallel for default(none) shared(size, src, dst)
    for (int i = 0; i < size; i++) {
        dst[i] = src[i];
    }
}

void sort_array(double m2[], int size) {
#ifdef _OPENMP
    double *arr2_omp = malloc(sizeof(double) * size);
#pragma omp parallel sections default(none) shared(m2, size)
    {
#pragma omp section
        comb_sort(m2, size / 2);
#pragma omp section
        comb_sort(m2 + size / 2, size - size / 2);
    }
    join_section_arrays(arr2_omp, m2, size / 2, m2 + size / 2, size - size / 2);
    copy_result(arr2_omp, m2, size);
#else
    comb_sort(m2, size);
#endif
}

unsigned int func(unsigned int i) {
    return i * i - 31 + 8 * log(i);
}

int main_loop(int argc, char *argv[]) {

    // 100 экспериментов
    for (unsigned int i = 0; i < K; i++) {
        //GENERATE:
        unsigned int tmp1 = i;
        unsigned int tmp2 = i;
        //Заполнить массив исходных данных размером N
        for (int j = 0; j < N; j++) {
            //  srand(func(tmp1));
            double value = 1 + rand_r(&tmp1) % (A - 1);
            m1[j] = value;
        }

        for (int j = 0; j < N / 2; j++) {
            //    srand(func(tmp2));
            double value = A + rand_r(&tmp2) % (A * 10 - A);
            m2[j] = value;
            m2_copy[j] = value;
        }


        //MAP: var 2 - гиперболический косинус с последующим увеличением на 1
#pragma omp parallel for default(none) shared(N, m1)
        for (int k = 0; k < N; k++) {
            m1[k] = cosh(m1[k]) + 1;
        }

        // var 4 - модуль котангенса
#pragma omp parallel for default(none) shared(N, m2, m2_copy)
        for (int k = 0; k < N / 2; k++) {
            if (k == 0) {
                m2[k] = fabs((double) 1 / tan(m2[k]));
            } else {
                m2[k] = fabs((double) 1 / tan(m2[k] + m2_copy[k - 1]));
            }
        }

        //Merge: var 2 - деление M2[i] = M[i]/M2[i]
#pragma omp parallel for default(none) shared(N, m1, m2)
        for (int k = 0; k < N / 2; k++) {
            m2[k] = (double) m1[k] / m2[k];
        }

        //Sort: var 2 - сортировка расческой
        sort_array(m2, N / 2);

        //Reduce:
        double result = 0;
        int j = 0;
        while (j < N / 2 && m2[j] == 0) {
            j++;
        }
        double min = m2[j];

#pragma omp parallel for default(none) shared(N, m2, min) reduction(+:result)
        for (int k = 0; k < N / 2; k++) {
            if (((long) (m2[k] / min) % 2) == 0) {
                result += sin(m2[k]);
            }
        }
        //printf("X: %f\n", result);
        *percent = (100 * (i + 1)) / K;
    }

    T2 = omp_get_wtime(); // запомнить текущее время T2
    delta_ms = (T2 - T1) * 1000;
//    printf("\nN=%d. Milliseconds passed: %ld\n", N, delta_ms); /* T2 - T1 */
    printf("%d;%ld\n", N, delta_ms); /* T2 - T1 */

    free(m1);
    free(m2);
    free(m2_copy);

    return 0;
}

void generate_part_m1(unsigned int tmp, int start_index, int len) {
    int counter = 0;
    while (counter < len) {
        double value = 1 + rand_r(&tmp) % (A - 1);
        m1[start_index + counter] = value;
    }
}

void generate_part_m2(unsigned int tmp, int start_index, int len) {
    int counter = 0;
    while (counter < len) {
        double value = 1 + rand_r(&tmp) % (A - 1);
        m2[start_index + counter] = value;
        m2_copy[start_index + counter] = value;
    }
}

void *main_function(void *arg) {
    int id = *((int *) arg);
    printf("%d", id);

    int delta_1 = N / THREAD_NUM;
    int start_i_1 = id * delta_1 + 1;
    if (N - (id * delta_1) > 2 * delta_1) { // middle of loop

    } else {// last part

    }

    int delta_2 = N / THREAD_NUM / 2;

    for (int i = 0; i < FOR_I; i++) {
        generate_part_m1(id * delta_1 + 1,);

        pthread_mutex_lock(&percent_mutex);
        *percent = (100 * (i + 1)) / FOR_I;
        pthread_mutex_unlock(&percent_mutex);
    }
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
    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    struct timeval T1, T2;
    long delta_ms;

    if (argc < 3) {
        printf("Need to add size of array and number of threads as input arguments\n");
        return -1;
    }

    N = atoi(argv[1]);
    THREADS_NUM = atoi(argv[2]);
    if (THREADS_NUM < 1) printf("threads_counter must be positive");
    if (argc >= 4) {
        FOR_I = atoi(argv[3]);
    } else FOR_I = 100;

    m2 = (double *) malloc(N * sizeof(double));
    m2 = (double *) malloc(N / 2 * sizeof(double));
    m2_copy = (double *) malloc(N / 2 * sizeof(double));

    *percent = (int) malloc(sizeof(int));
    *percent = 0;

    pthread_mutex_init(&percent_mutex, NULL);
    pthread_t thread[THREADS_NUM];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    gettimeofday(&T1, NULL);

    pthread_t tid_counter = thread[0];
    pthread_create(&tid_counter, &attr, percent_counter, argv[1]);
    pthread_join(tid_counter, NULL);

    for (int i = 1; i < THREADS_NUM; i++) {
        int id = i - 1;
        pthread_t tid = thread[i];
        pthread_create(&tid, &attr, main_function, &id);
        pthread_join(tid, NULL);
    }

    gettimeofday(&T2, NULL);
    delta_ms = 1000 * (T2.tv_sec - T1.tv_sec) + (T2.tv_usec - T1.tv_usec) / 1000;
    printf("%d;%ld\n", N, delta_ms);

    free(percent);
    free(m1);
    free(m2);
    free(m2_copy);
}
