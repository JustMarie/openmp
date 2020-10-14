#include <stdio.h>
#include <omp.h>
#include <cmath>

const unsigned int N = 100000;

double f(double x) 
{
    return sin(x);
}

int main ()
{
    int i;
    double a, b, h, sum, x, I;
    h = (b - a) / (double) N;
    sum = 0.0;
#pragma omp parallel reduction(+:sum) private (i,x) shared (n,h)
    int id = omp_get_thread_num();
    int numtr = omp_get_num_threads();
    for (i = id + 1; i < N; i=i+numtr)
    {
        x = h * i;
        sum += f(x);
    }
    I = h * sum;
    printf("Integral is approximately %.16f”, I);
    return 0;
}

