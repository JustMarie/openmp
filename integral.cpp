#include <stdio.h>
#include <omp.h>
#include <cmath>

const unsigned int N = 100000;

double f(double x)
{
	return sin(x);
}

void integral(int num_threads)
{
	double a, b;
	double h, x;

	double sum_parall, I_parall;
	double start_parall, end_parall;

	double sum_seq, I_seq;
	double start_seq, end_seq;
	int i;

	a = 0;
	b = 10;
	sum_parall = 0.0;
	sum_seq = 0.0;
	h = (b - a) / (double)N; //step of integration

	omp_set_dynamic(0);

#pragma omp parallel num_threads(num_threads)
	if (omp_get_thread_num() == 0)
	{
		printf("\n\nParallel execution with %d threads and %d elements\n", omp_get_num_threads(), N);
	}


	start_parall = omp_get_wtime();
#pragma omp parallel reduction(+:sum_parall) private (i, x) shared (h)
	{	
		int id = omp_get_thread_num();
		int numtr = omp_get_num_threads();
		for (i = id + 1; i < N; i = i + numtr)
		{
			x = h * i;
			sum_parall += f(x);
		}
	}
	I_parall = h * sum_parall;

	end_parall = omp_get_wtime();

	start_seq = omp_get_wtime();
	for (i = 0; i < N; i++)
	{
		x = h * i;
		sum_seq += f(x);
	}
	I_seq = h * sum_parall;
	end_seq = omp_get_wtime();

	printf("Parallel   : %f seconds\n", end_parall - start_parall);
	printf("Sequential : %f seconds\n", end_seq - start_seq);

	printf("Parallel integral   : %f\n", I_parall);
	printf("Sequential integral : %f\n", I_seq);
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
