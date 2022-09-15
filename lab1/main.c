#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void bubbleSort(int *data, size_t size) {
    size_t i, j;
    int tmp;
    for (i = 1; i < size; i++) {
        for (j = 1; j < size; j++) {
            if (data[j] < data[j-1]) {
                tmp = data[j];
                data[j] = data[j-1];
                data[j-1] = tmp;
            }
        }
    }
}

int main(int argc, char* argv[]) {
    int i, N;
    struct timeval T1, T2;
    long delta_ms;
    N = atoi(argv[1]); // N равен первому параметру командной строки
    gettimeofday(&T1, NULL); // запомнить текущее время T1

    // 100 экспериментов
    for (i=0; i<10; i++)  {
        srand(i); /* инициализировать начальное значение ГСЧ
	*/
        /* Заполнить массив исходных данных размером N */
        int *data = (int*) malloc(N * sizeof(int));
        int *result = (int*) malloc(N * sizeof(int));
        /* Решить поставленную задачу, заполнить массив с результатами
        */
        printf("Исходный: ");
        for (int j = 0; j < N; j++) {
            int value = rand();
            data[j] = value;
            result[j] = value;
            printf("%d ", value);
        }
        printf("\n");
        printf("Отсортированный: ");
        /* Отсортировать массив с результатами указанным методом */
        bubbleSort(result, N);

        for (int j = 0; j < N; j++) {
            printf("%d ", result[j]);
        }

        printf("\n");
        printf("\n");
        free(data);

    }

    gettimeofday(&T2, NULL); // запомнить текущее время T2
    delta_ms = 1000*(T2.tv_sec - T1.tv_sec) + (T2.tv_usec - T1.tv_usec) /1000;
    printf("\nN=%d. Milliseconds passed: %ld\n", N, delta_ms); /* T2 - T1 */

    return 0;
}