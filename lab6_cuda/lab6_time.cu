#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sys/time.h>

using namespace std;

const int A = 936;

__global__ void map_m1(double* m1_v, int size) {
    //линейный индекс потока
    unsigned int id = threadIdx.x + blockIdx.x * blockDim.x;
    //сколько один поток выполняет
    unsigned int threadsNum = blockDim.x * gridDim.x;
    for (unsigned int i = id; i < size; i += threadsNum) {
        m1_v[i] = cosh(m1_v[i]) + 1;
    }
}

__global__ void map_m2(double* m2_v, double* m2_copy_v, int size) {
    unsigned int id = threadIdx.x + blockIdx.x * blockDim.x;
    unsigned int threadsNum = blockDim.x * gridDim.x;
    for (unsigned int i = id; i < size; i += threadsNum) {
        if (i == 0) {
            m2_v[i] = fabs((double) 1 / tan(m2_v[i]));
        } else {
            m2_v[i] = fabs((double) 1 / tan(m2_v[i] + m2_copy_v[i - 1]));
        }
    }
}

__global__ void merge(const double* m1_v, double* m2_v, int size) {
    unsigned int id = threadIdx.x + blockIdx.x * blockDim.x;
    unsigned int threadsNum = blockDim.x * gridDim.x;
    for (unsigned int i = id; i < size; i += threadsNum) {
        m2_v[i] = (double) m1_v[i] / m2_v[i];
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
    int N, K;
    struct timeval T1, T2;
    long delta_ms;

    if (argc < 2) {
        printf("Need to add size of array as input arguments\n");
        return -1;
    }

    N = atoi(argv[1]);
    if (argc >= 3) {
        K = atoi(argv[2]);
    } else K = 100;

    double *m1, *m2, *m2_copy;
    double *m1v, *m2v, *m2_copyv;

    m1 = (double *) malloc(N * sizeof(double));
    m2 = (double *) malloc(N / 2 * sizeof(double));
    m2_copy = (double *) malloc(N / 2 * sizeof(double));

    //выделение памяти на устройстве
    cudaMalloc(&m1v, sizeof(double) * N);
    cudaMalloc(&m2v, sizeof(double) * N / 2);
    cudaMalloc(&m2_copyv, sizeof(double) * N / 2);

    gettimeofday(&T1, nullptr); //запомнить текущее время T1

    //для сихронизации потоков
    cudaEvent_t syncEvent;
    cudaEventCreate(&syncEvent);    //Создаем event
    cudaEventRecord(syncEvent, nullptr);  //Записываем event

    //расчёт gridSize и blockSize для m1
    int gridSize, blockSize, minGridSize;
    cudaOccupancyMaxPotentialBlockSize(&minGridSize, &blockSize, map_m1, 0, N);
    gridSize = (N + blockSize - 1) / blockSize;

    //расчёт gridSize и blockSize для m2
    int minGridSize2, blockSize2, gridSize2;
    cudaOccupancyMaxPotentialBlockSize(&minGridSize2, &blockSize2, map_m2, 0, N / 2);
    gridSize2 = (N / 2  + blockSize2 - 1) / blockSize2;

    long delta_gen, delta_map, delta_merge, delta_sort, delta_reduce;
    struct timeval T0, T_generate, T_map, T_merge, T_sort, T_result;

    for (unsigned int ink = 0; ink < K; ink++) {
        gettimeofday(&T0, nullptr);

        //======================GENERATE======================
        unsigned int tmp1 = ink;
        unsigned int tmp2 = ink;
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

        //копирование данных после инициализации
        cudaMemcpy(m1v, m1, sizeof(double) * N, cudaMemcpyHostToDevice);
        cudaMemcpy(m2v, m2, sizeof(double) * N / 2, cudaMemcpyHostToDevice);
        cudaMemcpy(m2_copyv, m2_copy, sizeof(double) * N / 2, cudaMemcpyHostToDevice);

        gettimeofday(&T_generate, nullptr);
        delta_gen = 1000 * (T_generate.tv_sec - T0.tv_sec) + (T_generate.tv_usec - T0.tv_usec) / 1000;




        //======================MAP======================

        map_m1<<<gridSize, blockSize>>>(m1v, N);
        map_m2<<<gridSize2, blockSize2>>>(m2v, m2_copyv, N / 2);

        //Хендл event'а
        cudaEventSynchronize(syncEvent);  //Синхронизируем event

        cudaMemcpy(m1, m1v, sizeof(double) * N, cudaMemcpyDeviceToHost);
        cudaMemcpy(m2, m2v, sizeof(double) * N / 2, cudaMemcpyDeviceToHost);

        gettimeofday(&T_map, nullptr);
        delta_map = 1000 * (T_map.tv_sec - T_generate.tv_sec) + (T_map.tv_usec - T_generate.tv_usec) / 1000;




        //======================MERGE======================
        merge<<<gridSize2, blockSize2>>>(m1v, m2v, N / 2);

        cudaEventSynchronize(syncEvent);  //Синхронизируем event
        cudaMemcpy(m2, m2v, sizeof(double) * N / 2, cudaMemcpyDeviceToHost);

        gettimeofday(&T_merge, nullptr);
        delta_merge = 1000 * (T_merge.tv_sec - T_map.tv_sec) + (T_merge.tv_usec - T_map.tv_usec) / 1000;





        //======================SORT(var 2 - сортировка расческой)======================
        comb_sort(m2, N / 2);
        cudaEventSynchronize(syncEvent);  //Синхронизируем event

        gettimeofday(&T_sort, nullptr);
        delta_sort = 1000 * (T_sort.tv_sec - T_merge.tv_sec) + (T_sort.tv_usec - T_merge.tv_usec) / 1000;




        //======================REDUCE======================
        reduce(m2, N / 2);

        gettimeofday(&T_result, nullptr);
        delta_reduce = 1000 * (T_result.tv_sec - T_sort.tv_sec) + (T_result.tv_usec - T_sort.tv_usec) / 1000;



        cout << delta_gen << ";" << delta_map << ";" << delta_merge << ";"
        << delta_merge << ";" << delta_sort << ";" << delta_reduce << endl;

        cudaEventSynchronize(syncEvent);  //Синхронизируем event
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