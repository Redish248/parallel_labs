#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>

const int A = 936;

void map_m1(double data[], int size) {
    for (int i = 0; i < size; i++) {
        data[i] = cosh(data[i]) + 1;
    }
}

void map_m2(double data[], int size) {
    double previous = 0;
    for (int i = 0; i < size; i++) {
        double tmp = data[i];
        data[i] = fabs((double) 1 / tan(data[i] + previous));
        previous = tmp;
    }
}

void merge(double m1[], double m2[], int size) {
    for (int i = 0; i < size; i++) {
        m2[i] = (double) m1[i] / m2[i];
    }
}

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

// data - sorted array !
double reduce(double data[], int size) {
    double result = 0;

    int j = 0;
    while (j < size && data[j] == 0) {
        j++;
    }
    double min = data[j];

    for (int i = 0; i < size; i++) {
        if (((long) (data[i] / min) % 2) == 0) {
            result += sin(data[i]);
        }
    }

    return result;
}

int main(int argc, char *argv[]) {
    int N;
    struct timeval T1, T2;
    long delta_ms;

    if (argc < 2) {
        printf("Need to add size of array as input argument\n");
        return -1;
    }

    N = atoi(argv[1]); // N равен первому параметру командной строки

    double *m1 = (double *) malloc(N * sizeof(double));
    double *m2 = (double *) malloc(N / 2 * sizeof(double));

    gettimeofday(&T1, NULL); // запомнить текущее время T1
    // 100 экспериментов
    for (unsigned int i = 0; i < 100; i++) {
        //Заполнить массив исходных данных размером N
        for (int j = 0; j < N; j++) {
            unsigned int tmp1 = i;
            double value = 1 + rand_r(&tmp1) % (A - 1);
            m1[j] = value;
        }

        for (int j = 0; j < N / 2; j++) {
            unsigned int tmp2 = i;
            double value = A + rand_r(&tmp2) % (A * 10 - A);
            m2[j] = value;
        }

        // Решить поставленную задачу, заполнить массив с результатами

        //MAP: var 2 - гиперболический косинус с последующим увеличением на 1
        map_m1(m1, N);
        // var 4 - модуль котангенса
        map_m2(m2, N / 2);

        //Merge: var 2 - деление M2[i] = M[i]/M2[i]
        merge(m1, m2, N / 2);

        //Sort: var 2 - сортировка расческой
        comb_sort(m2, N / 2);

        //Reduce:
        reduce(m2, N / 2);
    }

    gettimeofday(&T2, NULL); // запомнить текущее время T2
    delta_ms = 1000 * (T2.tv_sec - T1.tv_sec) + (T2.tv_usec - T1.tv_usec) / 1000;
//    printf("\nN=%d. Milliseconds passed: %ld\n", N, delta_ms); /* T2 - T1 */
    printf("%d;%ld\n", N, delta_ms); /* T2 - T1 */

    free(m1);
    free(m2);

    return 0;
}
