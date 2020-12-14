#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <iostream>
#include <ctime>

using namespace std;

void omp_max_el(int *arr, int N, int num_threads){

        int max_val_red = 0;
        int max_val_crit = 0;
        int max_val_seq = 0;
        int i;

        double start_red, end_red;
        double start_crit, end_crit;
        double start_seq, end_seq;

	
	// deprecate dynamic threads sheduling
        omp_set_dynamic(0);
#pragma omp parallel num_threads(num_threads)
        if (omp_get_thread_num() == 0)
        {
                printf("\n\nParallel execution with %d threads and %d elements\n", omp_get_num_threads());
        }
		       
		       
	// search with reduction directive
        start_red = omp_get_wtime();
#pragma omp parallel for reduction(max : max_val_red) num_threads(num_threads)
        for (i = 0;i < N; i++)
        {
                if (arr[i] > max_val_red)
                {
                        max_val_red = arr[i];
                }
        }
        end_red = omp_get_wtime();


// search with critical directive
        start_crit = omp_get_wtime();
#pragma omp parallel for shared(max_val_crit) num_threads(num_threads)
        for (i = 0;i < N; i++)
        {
                if (arr[i] > max_val_crit)
                {
#pragma omp critical
// second check since max_val_crit could have changed during the time while the thread enters the critical section 
// (this case can be observed with a small number of elements)
                        if (arr[i] > max_val_crit)
                        {
                                max_val_crit = arr[i];
                        }
                }
        }
        end_crit = omp_get_wtime();



// sequential code
        start_seq = omp_get_wtime();
        for (i = 0;i < N; i++)
        {
                if (arr[i] > max_val_seq)
                {
                        max_val_seq = arr[i];
                }
        }
        end_seq = omp_get_wtime();
		       
        printf("Reduction : %f seconds\n", end_red - start_red);
        printf("Critical  : %f seconds\n", end_crit - start_crit);
        printf("Sequential: %f seconds\n\n", end_seq - start_seq);

        printf("Reduction : %d \n", max_val_red);
        printf("Critical  : %d \n", max_val_crit);
        printf("Sequential: %d \n", max_val_seq);

        printf("\n=================================\n");
}


int main(int argc, char** argv)
{
         //const int N = atoi( argv[1]);
         const int N = 1000;

        inte *arr = (int*) malloc(N * sizeof(int));

        for (int i = 0; i < N; i++)
                arr[i] = rand() % 1000;

	omp_max_el(arr, N, 1);
	omp_max_el(arr, N, 2);
	omp_max_el(arr, N, 4);
        omp_max_el(arr, N, 8);
        omp_max_el(arr, N, 16);
	omp_max_el(arr, N, 32);
}
