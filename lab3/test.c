//
// Created by redish on 10.10.22.
//

#include <stdio.h>

#ifdef _OPENMP

#include "omp.h"

#else

void omp_set_num_threads(int M) {}

#endif

int m;
#pragma omp threadprivate(m)

int main() {
    /*
    int k, i;
#pragma omp parallel for
    for (i = 0; i < 5; i++)
        for (k = 0; k < 6; k++) {
#pragma omp critical
            printf("#%d - i=%d k=%d\n", omp_get_thread_num(), i, k);
        }
*/

    omp_set_num_threads(6);

#pragma omp parallel
    m = omp_get_thread_num();


    printf("m =%d ", ++m);
#pragma omp parallel
    {
        m = m * 2;
#pragma omp critical
        printf("%d - %d\n", omp_get_thread_num(), m);
    }
}







//#pragma omp parallel for
//    for (int i = 0; i < 5; i++)
//        for (int j = 0; j < 6; j++) {
//#pragma omp critical
//            printf("#%d - %d\n", omp_get_thread_num(), j);
//        }