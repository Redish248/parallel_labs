#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>
#include <time.h>

#ifdef _OPENMP
#include "omp.h"
#else
void omp_set_num_threads(int M) { }

double omp_get_wtime() {
    struct timeval T1;
    gettimeofday(&T1, NULL);
    return (double) T1.tv_sec + (double) T1.tv_usec / 1000000;
}

int omp_get_num_procs() { return 1; }
#endif

const int A = 936;

//void count_percent(const int *percent) {
//    int value;
//    for(;;) {
//        value = *percent;
//        printf("Current percent: %d\n", value);
//        if (value >= 100) break;
//        sleep(1);
//    }
//}

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

    while (i < size1)  {
        res_array[i_res++] = part1[i++];
    }
    while (j < size2)  {
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

int main_loop(int argc, char *argv[], int *percent) {
    int N, M, K;
    double T1, T2;
    long delta_ms;

    if (argc < 3) {
        printf("Need to add size of array and number of threads as input arguments\n");
        return -1;
    }

    N = atoi(argv[1]);
    M = atoi(argv[2]);
    if (argc >= 4) {
        K = atoi(argv[3]);
    } else K = 100;

    omp_set_num_threads(M);

    double *m1 = (double *) malloc(N * sizeof(double));
    double *m2 = (double *) malloc(N / 2 * sizeof(double));
    double *m2_copy = (double *) malloc(N / 2 * sizeof(double));

    T1 = omp_get_wtime();

    // 100 экспериментов
    for (unsigned int i = 0; i < K; i++) {
        //GENERATE:
        unsigned int tmp1 = i;
        unsigned int tmp2 = i;
        //Заполнить массив исходных данных размером N
        for (int j = 0; j < N; j++) {
            double value = 1 + rand_r(&tmp1) % (A - 1);
            m1[j] = value;
        }

        for (int j = 0; j < N / 2; j++) {
            double value = A + rand_r(&tmp2) % (A * 10 - A);
            m2[j] = value;
            m2_copy[j] = value;
        }

        //MAP: var 2 - гиперболический косинус с последующим увеличением на 1
#pragma omp parallel for default(none) shared(N, m1) schedule(SCHEDULE_TYPE, CHUNK_SIZE)
        for (int k = 0; k < N; k++) {
            m1[k] = cosh(m1[k]) + 1;
        }

        // var 4 - модуль котангенса
#pragma omp parallel for default(none) shared(N, m2, m2_copy) schedule(SCHEDULE_TYPE, CHUNK_SIZE)
        for (int k = 0; k < N / 2; k++) {
            m2[k] = fabs((double) 1 / tan(m2[k] + m2_copy[k]));
        }

        //Merge: var 2 - деление M2[i] = M[i]/M2[i]
#pragma omp parallel for default(none) shared(N, m1, m2) schedule(SCHEDULE_TYPE, CHUNK_SIZE)
        for (int k = 0; k < N / 2; k++) {
            m2[k] = (double) m1[k] / m2[k];
        }

        //Sort: var 2 - сортировка расческой
        comb_sort(m2, N / 2);

        //Reduce:
        double result = 0;
        int j = 0;
        while (j < N / 2 && m2[j] == 0) {
            j++;
        }
        double min = m2[j];
#pragma omp parallel for default(none) shared(N, m2, min) reduction(+:result) schedule(SCHEDULE_TYPE, CHUNK_SIZE)
        for (int k = 0; k < N / 2; k++) {
            if (((long) (m2[k] / min) % 2) == 0) {
                result += sin(m2[k]);
            }
        }
        //  printf("X: %f\n", result);
    }

    T2 = omp_get_wtime(); // запомнить текущее время T2
    delta_ms = (T2 - T1) / 1000;
//    printf("\nN=%d. Milliseconds passed: %ld\n", N, delta_ms); /* T2 - T1 */
    printf("%d;%ld\n", N, delta_ms); /* T2 - T1 */

    free(m1);
    free(m2);

    return 0;
}

int main(int argc, char *argv[]) {
    int *percent = malloc(sizeof(int));
    *percent = 0;
#ifdef _OPENMP
    omp_set_nested(1);
#pragma omp parallel sections default(none) shared(percent, argc, argv)
    {
//#pragma omp section
//        count_percent(percent);
#pragma omp section
        main_loop(argc, argv, percent);
    }
#else
    main_loop(argc, argv, percent);
#endif
    free(percent);
}
