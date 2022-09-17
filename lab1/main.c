#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

/* Combsort: function to find the new gap between the elements */
void combSort(int data[], int size) { //
    double factor = 1.2473309; // фактор уменьшения
    int step = size - 1; // шаг сортировки

    while (step >= 1) {
        for (int i = 0; i + step < size; i++) {
            if (data[i] > data[i + step]) {
                int tmp = data[i];
                data[i] = data[i + step];
                data[i + step] = tmp;
            }
        }
        step /= factor;
    }
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

        // TODO: Заполнить массив исходных данных размером N
        int *data = (int *) malloc(N * sizeof(int));
        int *result = (int *) malloc(N * sizeof(int));
        printf("Input array: ");
        for (int j = 0; j < N; j++) {
            int value = rand();
            data[j] = value;
            result[j] = value;
            printf("%d ", value);
        }
        printf("\n");
        printf("Sorted array: ");
        // TODO: Решить поставленную задачу, заполнить массив с результатами




        /* Отсортировать массив с результатами указанным методом */
        combSort(result, N);

        for (int j = 0; j < N; j++) {
            printf("%d ", result[j]);
        }

        printf("\n");
        printf("\n");
        free(data);
        free(result);

    }

    gettimeofday(&T2, NULL); // запомнить текущее время T2
    delta_ms = 1000 * (T2.tv_sec - T1.tv_sec) + (T2.tv_usec - T1.tv_usec) / 1000;
    printf("\nN=%d. Milliseconds passed: %ld\n", N, delta_ms); /* T2 - T1 */

    return 0;
}
