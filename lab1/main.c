#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

const int A = 43;

/* Combsort: function to find the new gap between the elements */
void combSort(double data[], int size) { //
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

void mapM1(double data[], int size) {
    int fix = 0;
    for (int i = 0; i < size; i++) {
        //TODO: +fix и есть с последующим уведичением на 1 или я не поняла?
        data[i] = cosh(data[i]) + fix;
        fix++;
    }
}

void mapM2(double data[], int size) {
    double previous = 0;
    for (int i = 0; i < size; i++) {
        double tmp = data[i];
        data[i] = fabs(1 / tan(data[i] + previous));
        previous =tmp ;
    }
}

void merge(double m1[], double m2[], int size) {
    for (int i = 0; i < size; i++) {
        m2[i] = m1[i]/m2[i];
    }
}

double reduce(double data[], int size) {
    double result = 0;

    int j = 0;
    while (data[j] == 0 && j < size) {
        j++;
    }
    double min = data[j];

    for (int i = 0; i < size; i++) {
        if (((long)(data[i] / min) % 2) == 0) {
            result += sin(data[i]);
        }
    }

    return result;
}

int main(int argc, char *argv[]) {
    int i, N;
    struct timeval T1, T2;
    long delta_ms;

    if (argc < 2) {
        printf("Need to add size of array as input argument");
        return -1;
    }

    N = atoi(argv[1]); // N равен первому параметру командной строки
    gettimeofday(&T1, NULL); // запомнить текущее время T1

    // 100 экспериментов
    for (i = 0; i < 100; i++) {
        srand(i); // инициализировать начальное значение ГСЧ

        //Заполнить массив исходных данных размером N
        double *m1 = (double*) malloc(N * sizeof(double));
        for (int j = 0; j < N; j++) {
            //FIXME: use rand_r
            double value = 1 + rand() % (A - 1);
            m1[j] = value;
        }

        double *m2 = (double*) malloc(N/2 * sizeof(double));
        for (int j = 0; j < N/2; j++) {
            double value = A + rand() % (A*10 - A);
            m2[j] = value;
        }

        // Решить поставленную задачу, заполнить массив с результатами

        //MAP:
        mapM1(m1, N);
        mapM2(m2, N/2);

        //Merge:
        merge(m1, m2, N/2);

        //Sort:
        combSort(m2, N/2);

        //Reduce:
        double result = reduce(m2, N/2);
        printf("X: %f\n", result);

        free(m1);
        free(m2);

    }

    gettimeofday(&T2, NULL); // запомнить текущее время T2
    delta_ms = 1000 * (T2.tv_sec - T1.tv_sec) + (T2.tv_usec - T1.tv_usec) / 1000;
    printf("\nN=%d. Milliseconds passed: %ld\n", N, delta_ms); /* T2 - T1 */

    return 0;
}
