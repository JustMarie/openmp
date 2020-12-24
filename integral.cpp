#include <stdio.h>
#include <omp.h>
#include <cmath>

const unsigned int N = 100000000;

double f(double x)
{
        return pow(tan(asinh(x)),3.0);
}

void integral(int num_threads)
{
        double a, b;
        double h, x;

        double sum_parall, I_parall;
        double sum_atomic, I_atomic;
        double start_parall, end_parall;
        double start_atomic, end_atomic;

        double sum_seq, I_seq;
        double start_seq, end_seq;
        int i;

        a = 0;
        b = 10;
        sum_parall = 0.0;
        sum_seq = 0.0;
        sum_atomic = 0.0;
        h = (b - a) / (double)N; //step of integration

        omp_set_dynamic(0);

#pragma omp parallel num_threads(num_threads)
        if (omp_get_thread_num() == 0)
        {
                printf("\n\nParallel execution with %d threads and %d elements\n", omp_get_num_threads()>
        }


        start_parall = omp_get_wtime();
#pragma omp parallel for reduction(+:sum_parall) private (i, x) num_threads(num_threads)
        for (i = 0; i < N; i++)
        {
                x = h * i;
                sum_parall += f(x);
        }

        I_parall = h * sum_parall;
        end_parall = omp_get_wtime();

        start_atomic = omp_get_wtime();
#pragma omp parallel for private(i, x)  num_threads(num_threads)
        for (i = 0; i < N; i++) {
                x = h * i;
#pragma omp atomic
                sum_atomic += f(x);
        }
        I_atomic = h * sum_atomic; 
        end_atomic = omp_get_wtime();

        start_seq = omp_get_wtime();
        for (i = 0; i < N; i++)
        {
                x = h * i;
                sum_seq += f(x);
        }
        I_seq = h * sum_parall;
        end_seq = omp_get_wtime();

        printf("%f\n", end_parall - start_parall);
        printf("%f\n", end_atomic - start_atomic);
        printf("%f\n", end_seq - start_seq);

//      printf("Parallel integral   : %f\n", I_parall);
//      printf("Atomic integral     : %f\n", I_atomic);
//      printf("Sequential integral : %f\n", I_seq);
}

int main(int argc, char** argv)
{

        integral(1);
        integral(2);
        integral(4);
        integral(8);
        integral(16);
        integral(32);
}


