#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>
#include <fwBase.h>
#include <fwSignal.h>

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
    int N, M;
    struct timeval T1, T2;
    long delta_ms;

    if (argc < 3) {
        printf("Need to add size of array and number of threads as input argument\n");
        return -1;
    }

    N = atoi(argv[1]); // N равен первому параметру командной строки
    M = atoi(argv[2]);

    fwSetNumThreads(M);
    gettimeofday(&T1, NULL); // запомнить текущее время T1

    // 100 экспериментов
    for (int i = 0; i < 100; i++) {
        srand(i); // инициализировать начальное значение ГСЧ

        //Заполнить массив исходных данных размером N
        double *m1 = (double *) malloc(N * sizeof(double));
        for (unsigned int j = 0; j < N; j++) {
            double value = 1 + rand_r(&j) % (A - 1);
            // double value = 1 + rand() % (A - 1);
            m1[j] = value;
        }

        double *m2 = (double *) malloc(N / 2 * sizeof(double));
        double *m2_copy = (double *) malloc(N / 2 * sizeof(double));
        for (unsigned int j = 0; j < N / 2; j++) {
            double value = A + rand_r(&j) % (A*10 - A);
            //double value = A + rand() % (A * 10 - A);
            m2[j] = value;
        }

        // Решить поставленную задачу, заполнить массив с результатами

        //MAP: M1 -- var 2 - гиперболический косинус с последующим увеличением на 1
        fwsCosh_64f_A50(m1, m1, N);
        fwsAddC_64f(m1, 1, m1, N);

        // M2 -- var 4 - модуль котангенса(элемент + предыдущий)

        //сначала копируем массив
        fwsCopy_64f(m2, m2_copy+1, N/2);
        m2_copy[0] = 0;

        //теперь abs(ctg(data+prev))
        fwsAdd_64f_I(m2_copy, m2, N/2);
        fwsTan_64f_A50(m2, m2, N/2);
        //FIXME: функция под x32!!!
        fwsDivCRev_32f_I(1, (Fw32f *) m2, N / 2);
        fwsAbs_64f(m2, m2, N/2);

        //Merge: var 2 - деление M2[i] = M[i]/M2[i]
        fwsDiv_64f(m1, m2,  m2, N / 2);

        //Sort: var 2 - сортировка расческой
        comb_sort(m2, N / 2);

        //Reduce:
        reduce(m2, N / 2);

        free(m1);
        free(m2);
    }

    gettimeofday(&T2, NULL); // запомнить текущее время T2
    delta_ms = 1000 * (T2.tv_sec - T1.tv_sec) + (T2.tv_usec - T1.tv_usec) / 1000;
//    printf("\nN=%d. Milliseconds passed: %ld\n", N, delta_ms); /* T2 - T1 */
    printf("%d;%ld\n", N, delta_ms); /* T2 - T1 */

    return 0;
}