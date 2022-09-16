#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

/* Combsort: function to find the new gap between the elements */
int newgap(int gap) {
    gap = (gap * 10) / 13;
    if (gap == 9 || gap == 10)
        gap = 11;
    if (gap < 1)
        gap = 1;
    return gap;
}

/* Combsort: implementation */
void combsort(int a[], int aSize) {
    int gap = aSize;
    double temp;
    int i;
    for (;;) {
        gap = newgap(gap);
        int swapped = 0;
        for (i = 0; i < aSize - gap; i++) {
            int j = i + gap;
            if (a[i] > a[j]) {
                temp = a[i];
                a[i] = a[j];
                a[j] = temp;
                swapped = 1;
            }
        }
        if (gap == 1 && !swapped)
            break;
    }
}

int main(int argc, char *argv[]) {
    int i, N;
    struct timeval T1, T2;
    long delta_ms;
    N = atoi(argv[1]); // N равен первому параметру командной строки
    gettimeofday(&T1, NULL); // запомнить текущее время T1

    // 100 экспериментов
    for (i = 0; i < 100; i++) {
        srand(i); // инициализировать начальное значение ГСЧ

        // TODO: Заполнить массив исходных данных размером N
        int *data = (int *) malloc(N * sizeof(int));
        int *result = (int *) malloc(N * sizeof(int));
        printf("Исходный: ");
        for (int j = 0; j < N; j++) {
            int value = rand();
            data[j] = value;
            result[j] = value;
            printf("%d ", value);
        }
        printf("\n");
        printf("Отсортированный: ");
        // TODO: Решить поставленную задачу, заполнить массив с результатами




        /* Отсортировать массив с результатами указанным методом */
        combsort(result, N);

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
