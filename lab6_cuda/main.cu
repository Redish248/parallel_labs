#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/time.h>

using namespace std;

const int A = 936;

__global__ void sum(double *a, double *b, double *c, int N) {
    int id = threadIdx.x + blockIdx.x * blockDim.x;
    int threadsNum = blockDim.x * gridDim.x;
    for (int i = id; i < N; i += threadsNum)
        c[i] = a[i] + b[i];
}

int main(int argc, char *argv[]) {
    int N, M, K;
    struct timeval T1, T2;
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
    double *m1, *m2, *m2_copy;
    double *m1v, *m2v, *m2_copyv;

    m1 = (double *) malloc(N * sizeof(double));
    m2 = (double *) malloc(N / 2 * sizeof(double));
    m2_copy = (double *) malloc(N / 2 * sizeof(double));

    cudaMalloc(&m1v, sizeof(double) * N);
    cudaMalloc(&m2v, sizeof(double) * N / 2);
    cudaMalloc(&m2_copyv, sizeof(double) * N / 2);

    gettimeofday(&T1, nullptr);

    // TODO: 100 экспериментов - вернуть цикл
    //GENERATE:
    unsigned int tmp1 = 10; //TODO: tmp1 = i;
    unsigned int tmp2 = 10; // tODO: tmp2 = i;
    //Заполнить массив исходных данных размером N
    for (int j = 0; j < N; j++) {
        double value = 1 + rand_r(&tmp1) % (A - 1);
        m1[j] = value;
    }

    for (int j = 0; j < N / 2; j++) {
        double value = A + rand_r(&tmp2) % (A * 10 - A);
        m2[j] = value;
       // m2_copy[j] = value;
    }

    for (int i = 0; i < N; i++) {
        cout << "m1 " << m1[i] << "\n";
    }

    for (int i = 0; i < N / 2; i++) {
        cout << "m2 " << m2[i] << "\n";
    }

    cudaMemcpy(m1v, m1, sizeof(double) * N, cudaMemcpyHostToDevice);
    cudaMemcpy(m2v, m2, sizeof(double) * N / 2, cudaMemcpyHostToDevice);
    cudaMemcpy(m2_copyv, m2_copy, sizeof(double) * N / 2, cudaMemcpyHostToDevice);

    dim3 gridSize = dim3(1, 1, 1);    //TODO: Размер используемого грида
    dim3 blockSize = dim3(N / 2, 1, 1); //TODO: Размер используемого блока

    sum<<<gridSize, blockSize>>>(m1v, m2v, m2_copyv, N / 2);

    //Хендл event'а
    cudaEvent_t syncEvent;

    cudaEventCreate(&syncEvent);    //Создаем event
    cudaEventRecord(syncEvent, nullptr);  //Записываем event
    cudaEventSynchronize(syncEvent);  //Синхронизируем event

    cudaMemcpy(m2_copy, m2_copyv, sizeof(double) * N / 2, cudaMemcpyDeviceToHost);

    for (int i = 0; i < N / 2; i++) {
        cout << m2_copy[i] << " ";
    }

    cudaEventDestroy(syncEvent);

    cudaFree(m1);
    cudaFree(m2);
    cudaFree(m2_copy);

    free(m1);
    free(m2);
    free(m2_copy);

    gettimeofday(&T2, nullptr); // запомнить текущее время T2
    delta_ms = 1000 * (T2.tv_sec - T1.tv_sec) + (T2.tv_usec - T1.tv_usec) / 1000;
   // printf("\nN=%d. Milliseconds passed: %ld\n", N, delta_ms); /* T2 - T1 */
    printf("%d;%ld\n", N, delta_ms); /* T2 - T1 */
    return 0;
}