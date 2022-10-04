#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

#ifdef _OPENMP
#include "omp.h"
#else
void omp_set_num_threads(int M) { }
#endif

const int A = 936;

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

int main(int argc, char *argv[]) {
    int N, M, chunk_size;
    char schedule_type[15];
    struct timeval T1, T2;
    long delta_ms;

    if (argc < 4) {
        printf("Need to add size of array, number of threads, schedule_type and chunk_size as input arguments\n");
        return -1;
    }

    N = atoi(argv[1]);
    M = atoi(argv[2]);
    sscanf(argv[3], "%s", schedule_type);
    chunk_size = atoi(argv[4]);

    omp_set_num_threads(M);

    double *m1 = (double *) malloc(N * sizeof(double));
    double *m2 = (double *) malloc(N / 2 * sizeof(double));

    gettimeofday(&T1, NULL); // запомнить текущее время T1
    // 100 экспериментов
    for (unsigned int i = 0; i < 100; i++) {
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
        }

        // Решить поставленную задачу, заполнить массив с результатами

        //MAP: var 2 - гиперболический косинус с последующим увеличением на 1
        #pragma omp parallel for default(none) shared(N, m1) schedule(schedule_type, chunk_size)
        for (int k = 0; k < N; k++) {
            m1[k] = cosh(m1[k]) + 1;
        }

        // var 4 - модуль котангенса
        double previous = 0;
        #pragma omp parallel for default(none) shared(N, m2, previous) schedule(schedule_type, chunk_size)
        for (int k = 0; k < N/2; k++) {
            double tmp = m2[k];
            m2[k] = fabs((double) 1 / tan(m2[k] + previous));
            previous = tmp;
        }

        //Merge: var 2 - деление M2[i] = M[i]/M2[i]
        #pragma omp parallel for default(none) shared(N, m1, m2) schedule(schedule_type, chunk_size)
        for (int k = 0; k < N/2; k++) {
            m2[k] = (double) m1[k] / m2[k];
        }

        //Sort: var 2 - сортировка расческой
        comb_sort(m2, N / 2);

        //Reduce:
        double result = 0;
        int j = 0;
        while (j < N/2 && m2[j] == 0) {
            j++;
        }
        double min = m2[j];
        #pragma omp parallel for default(none) shared(N, m2, min) reduction(+:result) schedule(schedule_type, chunk_size)
        for (int k = 0; k < N/2; k++) {
            if (((long) (m2[k] / min) % 2) == 0) {
                result += sin(m2[k]);
            }
        }
        //  printf("X: %f\n", result);
    }

    gettimeofday(&T2, NULL); // запомнить текущее время T2
    delta_ms = 1000 * (T2.tv_sec - T1.tv_sec) + (T2.tv_usec - T1.tv_usec) / 1000;
//    printf("\nN=%d. Milliseconds passed: %ld\n", N, delta_ms); /* T2 - T1 */
    printf("%d;%ld\n", N, delta_ms); /* T2 - T1 */

    free(m1);
    free(m2);

    return 0;
}
