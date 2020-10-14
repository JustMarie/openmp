#include <stdio.h>
#include <omp.h>

const unsigned int N = 100000;

int main ()
{
    int i;
    double a, b, h, sum, x, I;
    h = (b - a) / (double) N;
    sum = 0.0;
#pragma omp parallel for reduction(+:sum) private(x)
    for (i = 0; i < N; i ++)
    {
        x = h * i;
        sum += f(x);
    }
    I = h * sum;
    printf("Integral is approximately %.16f”, I);
    return 0;
}
