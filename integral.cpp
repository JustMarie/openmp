#include <stdio.h>
#include <omp.h>
#include <cmath>
#include <chrono>

using namespace std::chrono;

const unsigned int N = 100000;
const unsigned NUM_THREADS = 4;

double f(double x)
{
	return sin(x);
}

int main()
{
	int i;
	double h, sum, x, I;
	double a = 0, b = 1;
	h = (b - a) / (double)N;
	sum = 0.0;

	auto start = high_resolution_clock::now();

	omp_set_num_threads(NUM_THREADS);

#pragma omp parallel reduction(+:sum) private (i,x) shared (n,h)
	{	int id = omp_get_thread_num();
		int numtr = omp_get_num_threads();
		for (i = id + 1; i < N; i = i + numtr)
		{
			x = h * i;
			sum += f(x);
		}
	}
	I = h * sum;

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);

	printf("Elapsed time %lld  microseconds \n", duration.count());
	printf("Integral is approximately %.16f \n", I);
	return 0;
}

