#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <cmath>
#include <iostream>
#include <ctime>
#include <chrono>
#include <algorithm> 
#include <vector> 

using namespace std;
// Addition and min
void customReduction(std::vector<float> a, int num_threads, int N)
{
        // vars for atomic
        float sum_atomic, sum_pr_atomic;
        double start_atomic, end_atomic;

        // vars for critical 
        float sum_critical, sum_pr_critical;
        double start_critical, end_critical;



        // vars for locks
        float sum_lock, sum_pr_lock;
        double start_lock, end_lock;

        // vars for sequential execution
        float sum_seq;
        double start_seq, end_seq;

        int i;

        sum_atomic = 0.0f;
        sum_critical = 0.0f;
        sum_lock = 0.0f;
        sum_seq = 0.0f;

        omp_set_dynamic(0);

#pragma omp parallel num_threads(num_threads)
        if (omp_get_thread_num() == 0)
        {
                printf("\n\nParallel execution with %d threads and %d elements\n", omp_get_num_threads(), N);
        }


        // Atomic operation

        start_atomic = omp_get_wtime();
#pragma omp parallel shared(sum_atomic, a) private(sum_pr_atomic) num_threads(num_threads)
        {
                sum_pr_atomic = 0.0f;
#pragma omp for private(i) nowait
                for (i = 0; i < N; i++) {
                        sum_pr_atomic += a[i];
                }
#pragma omp atomic 
                sum_atomic += sum_pr_atomic;
        }
        end_atomic = omp_get_wtime();


        // Critical operation

        start_critical = omp_get_wtime();
#pragma omp parallel shared(sum_critical, a) private(sum_pr_critical) num_threads(num_threads)
        {
                sum_pr_critical = 0.0f;
#pragma omp for private(i) nowait
                for (i = 0; i < N; i++) {
                        sum_pr_critical += a[i];
                }
#pragma omp critical
                {
                        sum_critical += sum_pr_critical;
                }
        }
        end_critical = omp_get_wtime();


        // Locks
        omp_lock_t lock;
        omp_init_lock(&lock);

        start_lock = omp_get_wtime();
#pragma omp parallel shared(sum_lock, a) private(sum_pr_lock) num_threads(num_threads)
        {
                sum_pr_lock = 0.0f;
#pragma omp for private(i) nowait
                for (i = 0; i < N; i++) {
                        sum_pr_lock += a[i];
                }

                omp_set_lock(&lock);
                        sum_lock += sum_pr_lock;
                omp_unset_lock(&lock);
        }
        end_lock = omp_get_wtime();
        omp_destroy_lock(&lock);


        // Sequential execution

        start_seq = omp_get_wtime();
        for (i = 0; i < N; i++) {
                sum_seq = sum_seq + a[i];
        }
        end_seq = omp_get_wtime();

        printf("Atomic  : %f seconds\n", end_atomic - start_atomic);
        printf("Critical: %f seconds\n", end_critical - start_critical);
        printf("Locks   : %f seconds\n", end_lock - start_lock);
        printf("Seq     : %f seconds\n", end_seq - start_seq);

        printf("\nAtomic sum  : %f\n", sum_atomic);
        printf("Critical sum: %f\n", sum_critical);
        printf("Lock sum    : %f\n", sum_lock);
        printf("Seq sum     : %f\n", sum_seq);

        printf("\n=======================================================\n");
}

int main(int argc, char** argv)
{
        const int N = atoi(argv[1]);
        vector<float> a(N);

        std::srand(time(0));
        for (int i = 0; i < N; i++) {
                a[i] = (rand() % 1000) /10.0;
        }

        customReduction(a, 1, N);
        customReduction(a, 2, N);
        customReduction(a, 4, N);
        customReduction(a, 8, N);
        customReduction(a, 16, N);
}
