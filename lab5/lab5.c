#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

struct main_loop_params {
    int N;
    int M;
    int K;
    int* percent;
};

struct thread_params {
    int chunk_size;
    int thread_id;
    int num_threads;
};

struct generate_params {
    double* array;
    int size;
    unsigned int seed;
    int min;
    int max;
    struct thread_params thread_p;
};

struct copy_params {
    double* src;
    double* dst;
    int size;
    struct thread_params thread_p;
};

struct map_m1_params {
    unsigned int size;
    double* m1;
    struct thread_params thread_p;
};

struct map_m2_merge_params {
    unsigned int N;
    double* array1;
    double* array2;
    struct thread_params thread_p;
};

struct sort_params {
    double* m2;
    int size;
};

struct reduce_params {
    unsigned int N;
    double* m2;
    double min;
    double res;
    struct thread_params thread_p;
};

const int A = 936;

void* count_percent(void* percent_input) {
    const int* percent = (int*) percent_input;
    int value;
    for(;;) {
        value = *percent;
        printf("Current percent: %d\n", value);
        if (value >= 100) break;
        sleep(1);
    }
    pthread_exit(NULL);
}

void* generate(void* params_p) {
    struct generate_params *params = (struct generate_params*) params_p;
    int chunk_size = params->thread_p.chunk_size;
    int thread_id = params->thread_p.thread_id;
    int num_threads = params->thread_p.num_threads;

    for (int i = thread_id * chunk_size; i < params->size; i += num_threads * chunk_size) {
        for (int j = 0; i + j < params->size && j < chunk_size; j++) {
            int next = i + j;
            double value = params->min + rand_r(&params->seed) % params->max;
            params->array[next] = value;
        }
    }
    pthread_exit(NULL);
}

void generate_pthread(double* array, unsigned int seed, int chunk_size,
                      int size, int min, int max, int M) {
    struct generate_params params[M];
    pthread_t threads[M];
    for (int i = 0; i < M; i++) {
        params[i].array = array;
        params[i].size = size;
        params[i].seed = seed;
        params[i].min = min;
        params[i].max = max;
        params[i].thread_p.chunk_size = chunk_size;
        params[i].thread_p.thread_id = i;
        params[i].thread_p.num_threads = M;
        pthread_create(&threads[i], NULL, generate, &params[i]);
    }
    for (int i = 0; i < M; i++) {
        pthread_join(threads[i], NULL);
    }
}

void* array_copy(void* params_p) {
    struct copy_params *params = (struct copy_params*) params_p;

    int chunk_size = params->thread_p.chunk_size;
    int thread_id = params->thread_p.thread_id;
    int num_threads = params->thread_p.num_threads;

    for (int i = thread_id * chunk_size; i < params->size; i += num_threads * chunk_size) {
        for (int j = 0; i + j < params->size && j < chunk_size; j++) {
            int next = i + j;
            params->dst[next] = params->src[next];
        }
    }

    pthread_exit(NULL);
}

void array_copy_pthread(double *src, double *dst, int size, int M) {
    struct copy_params params[M];
    pthread_t threads[M];
    for (int i = 0; i < M; i++) {
        params[i].src = src;
        params[i].dst = dst;
        params[i].size = size;
        params[i].thread_p.chunk_size = (i == M - 1) ? (size - (i - 1) * size / M) : (size / M);
        params[i].thread_p.thread_id = i;
        params[i].thread_p.num_threads = M;
        pthread_create(&threads[i], NULL, array_copy, &params[i]);
    }
    for (int i = 0; i < M; ++i)  {
        pthread_join(threads[i], NULL);
    }
}

void* map_m1(void* params_p) {
    struct map_m1_params *params = (struct map_m1_params*) params_p;

    int chunk_size = params->thread_p.chunk_size;
    int thread_id = params->thread_p.thread_id;
    int num_threads = params->thread_p.num_threads;

    for (int i = thread_id * chunk_size; i < params->size; i += num_threads * chunk_size) {
        for (int j = 0; i + j < params->size && j < chunk_size; j++) {
            int next = i + j;
            params->m1[next] = cosh(params->m1[next]) + 1;
        }
    }

    pthread_exit(NULL);
}

void map_m1_pthread(double* m1, int size, int M) {
    struct map_m1_params params[M];
    pthread_t threads[M];
    for (int i = 0; i < M; i++) {
        params[i].m1 = m1;
        params[i].size = size;
        params[i].thread_p.chunk_size = (i == M - 1) ? (size - (i - 1) * size / M) : (size / M);
        params[i].thread_p.thread_id = i;
        params[i].thread_p.num_threads = M;
        pthread_create(&threads[i], NULL, map_m1, &params[i]);
    }
    for (int i = 0; i < M; i++) {
        pthread_join(threads[i], NULL);
    }
}

void* map_m2(void* params_p) {
    struct map_m2_merge_params *params = (struct map_m2_merge_params*) params_p;
    int chunk_size = params->thread_p.chunk_size;
    int thread_id = params->thread_p.thread_id;
    int num_threads = params->thread_p.num_threads;

    for (int i = thread_id * chunk_size; i < params->N; i += num_threads * chunk_size) {
        for (int j = 0; i + j < params->N && j < chunk_size; j++) {
            int next = i + j;
            params->array1[next] = fabs((double) 1 / tan(params->array1[next] + params->array2[next]));
        }
    }
    pthread_exit(NULL);
}

void map_m2_pthread(double* m2, double* m2_copy, int size, int M) {
    struct map_m2_merge_params params[M];
    pthread_t threads[M];
    for (int i = 0; i < M; i++) {
        params[i].N = size;
        params[i].array1 = m2;
        params[i].array2 = m2_copy;
        params[i].thread_p.chunk_size = size  / M;
        params[i].thread_p.thread_id = i;
        params[i].thread_p.num_threads = M;
        pthread_create(&threads[i], NULL, map_m2, &params[i]);
    }
    for (int i = 0; i < M; i++) {
        pthread_join(threads[i], NULL);
    }
}

void* merge(void* params_p) {
    struct map_m2_merge_params *params = (struct map_m2_merge_params*) params_p;
    int chunk_size = params->thread_p.chunk_size;
    int thread_id = params->thread_p.thread_id;
    int num_threads = params->thread_p.num_threads;

    for (int i = thread_id * chunk_size; i < params->N; i += num_threads * chunk_size) {
        for (int j = 0; i + j < params->N && j < chunk_size; j++) {
            int next = i + j;
            params->array2[next] = (double) params->array1[next] / params->array2[next];
        }
    }
    pthread_exit(NULL);
}

void merge_pthread(double* m1, double* m2, int size, int M) {
    struct map_m2_merge_params params[size];
    pthread_t threads[M];
    for (int i = 0; i < M; i++) {
        params[i].N = size;
        params[i].array1 = m1;
        params[i].array2 = m2;
        params[i].thread_p.chunk_size = (i == M - 1) ? (size - (i - 1) * size / M) : (size / M);
        params[i].thread_p.thread_id = i;
        params[i].thread_p.num_threads = M;
        pthread_create(&threads[i], NULL, merge, &params[i]);
    }
    for (int i = 0; i < M; i++) {
        pthread_join(threads[i], NULL);
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

void join_section_arrays(double *res_array, const double *part1, int size1, const double *part2, int size2) {
    int i = 0, j = 0, i_res = 0;

    for (; i < size1 && j < size2;) {
        if (part1[i] < part2[j]) {
            res_array[i_res++] = part1[i++];
        } else {
            res_array[i_res++] = part2[j++];
        }
    }

    while (i < size1) {
        res_array[i_res++] = part1[i++];
    }
    while (j < size2) {
        res_array[i_res++] = part2[j++];
    }
}

void* sort(void* params_p) {
    struct sort_params *params = (struct sort_params*) params_p;

    comb_sort(params->m2, params->size);

    pthread_exit(NULL);
}

void sort_pthread(double* m2, int size, int M) {
    double *m2_sorted = malloc(sizeof(double) * size * 2);

    pthread_t threads_sort[2];
    struct sort_params params[2];
    params[0].m2 = m2;
    params[0].size = size / 2;
    pthread_create(&threads_sort[0], NULL, sort, &params[0]);
    params[1].m2 = m2 + size / 2;
    params[1].size = size - size / 2;
    pthread_create(&threads_sort[1], NULL, sort, &params[1]);
    pthread_join(threads_sort[0], NULL);
    pthread_join(threads_sort[1], NULL);
    join_section_arrays(m2_sorted, m2, size, m2 + size / 2, size - size / 2);
    array_copy_pthread(m2_sorted, m2, size, M);

    free(m2_sorted);
}

void* reduce(void* params_p) {
    struct reduce_params *params = (struct reduce_params*) params_p;
    double res = params->res;
    int chunk_size = params->thread_p.chunk_size;
    int thread_id = params->thread_p.thread_id;
    int num_threads = params->thread_p.num_threads;

    for (int i = thread_id * chunk_size; i < params->N; i += num_threads * chunk_size) {
        for (int j = 0; i + j < params->N && j < chunk_size; j++) {
            int next = i + j;
            if ((int) (params->m2[next] / params->min) % 2 == 0) {
                res += sin(params->m2[next]);
            }
        }
    }
    params->res = res;
    pthread_exit(NULL);
}

double reduce_pthread(double* m2, int size, int M) {
    double X = 0;

    int k = 0;
    while (k < size && m2[k] == 0) {
        k++;
    }
    double min = m2[k];

    if (min == 0) {
        exit(0);
    }
    struct reduce_params thread_params[M];
    pthread_t threads[M];
    for (int i = 0; i < M; i++) {
        thread_params[i].N = size;
        thread_params[i].m2 = m2;
        thread_params[i].min = min;
        thread_params[i].res = 0;
        thread_params[i].thread_p.chunk_size = (i == M - 1) ? (size - (i - 1) * size / M) : (size / M);
        thread_params[i].thread_p.thread_id = i;
        thread_params[i].thread_p.num_threads = M;
        pthread_create(&threads[i], NULL, reduce, &thread_params[i]);
    }
    for (int i = 0; i < M; i++) {
        pthread_join(threads[i], NULL);
        X += thread_params[i].res;
    }

    return X;
}

void* main_loop(void* full_params) {
    int N, M, K;
    struct timeval T1, T2;
    long delta_ms;
    int* percent = ((struct main_loop_params*)full_params)->percent;

    struct main_loop_params *params = (struct main_loop_params *) full_params;

    N = params->N;
    M = params->M;
    K = params->K;

    double *m1 = (double *) malloc(N * sizeof(double));
    double *m2 = (double *) malloc(N / 2 * sizeof(double));
    double *m2_copy = (double *) malloc(N / 2 * sizeof(double));

    gettimeofday(&T1, NULL); /* запомнить текущее время T1 */

    // 100 экспериментов
    for (unsigned int i = 0; i < K; i++) {
        //GENERATE:
        unsigned int tmp1 = i;
        unsigned int tmp2 = i;
        //Заполнить массив исходных данных размером N
        generate_pthread(m1, tmp1, N/M, N, 1, A - 1, M);
        generate_pthread(m2, tmp2, N/2/M, N/2, A, A*10 - A, M);

        //copy m1
        array_copy_pthread(m2, m2_copy, N/2, M);

        //MAP: var 2 - гиперболический косинус с последующим увеличением на 1
        map_m1_pthread(m1, N, M);

        // var 4 - модуль котангенса
        map_m2_pthread(m2, m2_copy, N / 2, M);

        //Merge: var 2 - деление M2[i] = M[i]/M2[i]
        merge_pthread(m1, m2, N / 2, M);

        //Sort: var 2 - сортировка расческой
        sort_pthread(m2, N / 2, M);

        //Reduce:
        double result = reduce_pthread(m2, N / 2, M);
        printf("X: %f\n", result);

        *percent = (100 * (i + 1)) / K;
    }

    gettimeofday(&T2, NULL); // запомнить текущее время T2
    delta_ms = 1000 * (T2.tv_sec - T1.tv_sec) + (T2.tv_usec - T1.tv_usec) / 1000;
//    printf("\nN=%d. Milliseconds passed: %ld\n", N, delta_ms); /* T2 - T1 */
    printf("%d;%ld\n", N, delta_ms); /* T2 - T1 */

    free(m1);
    free(m2);
    free(m2_copy);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int *percent = malloc(sizeof(int));
    *percent = 0;
    pthread_t threads[2];
    struct main_loop_params params;

    if (argc < 3) {
        printf("Need to add size of m1 and number of threads as input arguments\n");
        return -1;
    }

    params.N = atoi(argv[1]);
    params.M = atoi(argv[2]);
    if (argc >= 4) {
        params.K = atoi(argv[3]);
    } else params.K = 100;
    params.percent = percent;

    pthread_create(&threads[0], NULL, count_percent, percent);
    pthread_create(&threads[1], NULL, main_loop, &params);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    free(percent);
}
