#include <stdio.h>
#include <omp.h>
#include <cmath>
#include <chrono>
#include <algorithm> 
#include <vector> 

using namespace std;
using namespace std::chrono;

const unsigned int N = 10000000;
const unsigned NUM_THREADS = 4;


int main()
{
	vector<int> values1(N);
	vector<int> values2(N);

	int i, sum = 0.0;

	auto f = []() -> int { return rand() % 10000; };
	generate(values1.begin(), values1.end(), f);
	generate(values2.begin(), values2.end(), f);

	omp_set_num_threads(NUM_THREADS);

	auto start = high_resolution_clock::now();

#pragma omp parallel reduction(+:sum) private (i)
	{	int id = omp_get_thread_num();
		int numtr = omp_get_num_threads();
		for (i = id + 1; i < N; i = i + numtr)
		{
			sum += values1[i] * values2[i];
		}
	}

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);

	printf("Elapsed time %lld  microseconds \n", duration.count());
	// printf("dot product of vectors %d \n", sum);
	return 0;
}
